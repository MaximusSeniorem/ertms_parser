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

        constexpr std::size_t bit_size() { 
            return (m_offset%CHAR_BIT == 0) ? m_offset/CHAR_BIT : m_offset/CHAR_BIT + 1;
        }  
        
        constexpr std::size_t bit_capacity() { 
            return m_bsize;
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
            //oob
            if(this->m_offset + val_len >= this->m_bsize){
                //hande buffer read oob error, throws(--) or internal error at buffer level
                return *this;
            }

            const std::size_t buf_idx = this->m_offset / CHAR_BIT;
            std::size_t buf_shift = this->m_offset % CHAR_BIT;
            T *buf = (T*) &this->m_buf[buf_idx];
            
            val = *buf;

            const std::size_t t_sz = sizeof(T);
            const std::uint8_t t_len = t_sz * CHAR_BIT;
            auto val_ptr = std::as_writable_bytes(std::span{&val, 1});

            std::size_t val_last_index = t_sz - 1;
            if constexpr (std::endian::native == std::endian::little){
                val = std::byteswap(val);
                val_last_index = 0;
            }

            val <<= buf_shift;
            val >>= (t_len - val_len);

            //check if there's bits left to read
            buf_shift = buf_shift + val_len - t_len;
            if(buf_shift > 0)
                val_ptr[val_last_index] |= this->m_buf[t_sz + buf_idx] >> (CHAR_BIT - buf_shift);
            
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
            //oob
            if(this->m_offset + val_len >= this->m_bsize){
                //hande buffer overflow error, throws(--) or internal error at buffer level
                return *this;
            }

            const std::size_t buf_idx = this->m_offset / CHAR_BIT;
            std::size_t buf_shift = this->m_offset % CHAR_BIT;

            const std::size_t t_sz = sizeof(T);
            const std::size_t t_len = t_sz * CHAR_BIT;
            T * buf = (T *)&this->m_buf[buf_idx];

            T val_shifted = val << (t_len - val_len);
            auto val_ptr = std::as_bytes(std::span{&val_shifted, 1});

            /* apply buffer shift */
            val_shifted >>= buf_shift;

            /* handle endianess: buffer is big-endian */
            std::size_t last_index = t_sz - 1;
            if constexpr (std::endian::native == std::endian::little){
                val_shifted = std::byteswap(val_shifted);
                last_index = 0;
            }
            
            *buf |= val_shifted;

            //check if there's bits left to write
            buf_shift = buf_shift + val_len - t_len;
            if (buf_shift > 0){
                val_shifted = val << CHAR_BIT - buf_shift;
                this->m_buf[t_sz + buf_idx] |= val_ptr[last_index];
            }
        
            this->m_offset += val_len;

            return *this;
        }
    };
}