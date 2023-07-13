#pragma once

#include <climits>
#include <cstdint>
#include <span>
#include <bit>


namespace bb{
    struct bitbuffer{
        bitbuffer(std::span<std::byte> buf) : 
            m_buf{buf}, m_bsize{buf.size() * CHAR_BIT}, m_offset{0} 
        {}
        
        virtual ~bitbuffer() = default;

        std::size_t bit_size(){ 
            return (m_offset%CHAR_BIT == 0) ? m_offset/CHAR_BIT : m_offset/CHAR_BIT + 1;
        }

    protected:
        std::span<std::byte> m_buf;
        std::size_t m_bsize;
        std::size_t m_offset;
    };


    struct ibitstream : bitbuffer{
        ibitstream(std::span<std::byte> buf) :
            bitbuffer(buf) 
        {}

        template <typename T>
        ibitstream& read(T & val, std::size_t val_len){
            std::size_t index = this->m_offset / CHAR_BIT;
            const std::size_t val_size = sizeof(T);
            std::uint8_t t_len = val_size * CHAR_BIT;
            auto val_ptr = std::as_writable_bytes(std::span{&val, 1});
            
            //oob
            if(this->m_offset + val_len >= this->m_bsize){
                //hande buffer read oob error, throws(--) or internal error at buffer level
                return *this;
            }

            for (auto i = 0; i < val_size; ++i){
                val_ptr[i] = this->m_buf[i + index];
            }

            std::size_t val_last_index = val_size - 1;
            if constexpr (std::endian::native == std::endian::little){
                val = std::byteswap(val);
                val_last_index = 0;
            }
             
            std::uint8_t loc_shift = this->m_offset % CHAR_BIT;
            val <<= loc_shift;
            val >>= (t_len - val_len);

            //check if there's bits left to read
            loc_shift = loc_shift + val_len - t_len;
            if(loc_shift > 0)
                val_ptr[val_last_index] |= this->m_buf[val_size + index] >> (CHAR_BIT - loc_shift);
            
            this->m_offset += val_len;
            return *this;
        }
    };


    struct obitstream : bitbuffer{
        obitstream(std::span<std::byte> buf) :
            bitbuffer(buf)
        {}

        template <typename T>
        obitstream& write(T val, std::size_t val_len){
            const std::size_t index = this->m_offset / CHAR_BIT;
            std::uint8_t loc_shift = this->m_offset % CHAR_BIT;
            const std::size_t val_size = sizeof(T);
            const std::uint8_t t_len = val_size * CHAR_BIT;
            T val_shifted = val << (t_len - val_len);
            auto val_ptr = std::as_bytes(std::span{&val_shifted, 1});

            //buffer overflow
            if(this->m_offset + val_len >= this->m_bsize){
                //hande buffer overflow error, throws(--) or internal error at buffer level
                return *this;
            }

            val_shifted >>= loc_shift;
            std::size_t val_last_index = val_size - 1;
            if constexpr (std::endian::native == std::endian::little){
                val_shifted = std::byteswap(val_shifted);
                val_last_index = 0;
            }
            for (auto i = 0; i < val_size; ++i){
                this->m_buf[index + i] |= val_ptr[i];
            }

            //check if there's bits left to write
            loc_shift = loc_shift + val_len - t_len;
            if (loc_shift > 0){
                val_shifted = val << CHAR_BIT - loc_shift;
                this->m_buf[val_size + index] |= val_ptr[val_last_index];
            }
        
            this->m_offset += val_len;

            return *this;
        }    
    };
}