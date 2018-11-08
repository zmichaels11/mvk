#pragma once

#include <cstddef>

#include "volk.h"
#include "vk_mem_alloc.h"

#include <functional>
#include <memory>
#include <set>
#include <utility>

#include "mvk/BufferUsageFlag.hpp"
#include "mvk/MemoryUsage.hpp"
#include "mvk/SharingMode.hpp"

namespace mvk {
    class Device;
    class QueueFamily;

    class Buffer;

    using UPtrBuffer = std::unique_ptr<Buffer>;

    //! Vulkan Buffer object.
    /*!
        Buffers represent linear arrays of data which are used for various purposes 
        by binding them to a graphics or compute pipeline via descriptor sets or via certain commands, 
        or by directly specifying them as parameters to certain commands.

        See: <a href="https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkBuffer.html">VkBuffer</a>
     */ 
    class Buffer {
    public:
        //! Parameter structure specifying how to construct a new Buffer object.
        struct CreateInfo {            
            unsigned int flags;                         /*!< Bitmask of additional buffer parameters. */
            VkDeviceSize size;                          /*!< The size of the Buffer, in bytes. */
            BufferUsageFlag usage;                      /*!< Bitmask specifying usage limitations of the Buffer. */
            SharingMode sharingMode;                    /*!< Sharing limitations for the Buffer. */
            std::set<QueueFamily * > queueFamilies;     /*!< Set of QueueFamily objects that can use the Buffer. */
            bool exported;                              /*!< Specifies if the memory should be exported. */         
        };

        //! Constant used when referring to the entire length of a Buffer.
        static constexpr VkDeviceSize WHOLE_SIZE = VK_WHOLE_SIZE;

        //! Constructs a Buffer-typed unique_ptr pointing to null. 
        /*!
            \return unique_ptr<Buffer> pointing to nullptr.
         */
        static inline UPtrBuffer unique_null() {
            return std::unique_ptr<Buffer> ();
        }

    private:
        Device * _device;
        VkBuffer _handle;
        CreateInfo _info;

        union {
            VmaAllocation local;
            VkDeviceMemory shared;
        } _memory;

        Buffer(const Buffer&) = delete;
        Buffer& operator= (const Buffer&) = delete;
        
    public:
        //! User-specified pointer.
        /*!
            This allows the user to annotate a Buffer Object with metadata.
         */
        std::shared_ptr<void> userData;

        //! Constructs a null Buffer.
        /*!
            Constructs a Buffer-wrapper for nothing. 
            The resulting buffer is not valid for use at all.
         */
        Buffer() noexcept:
            _device(nullptr),
            _handle(VK_NULL_HANDLE) {}

        //! Constructs a new Buffer.
        /*!
            \param device is the Vulkan Device; used to create the Buffer.
            \param info is the parameter struct used to build the Buffer.
            \param memoryUsage is the requested memory layout
         */
        Buffer(Device * device, const CreateInfo& info, MemoryUsage memoryUsage);

        //! Move-constructs a Buffer object
        /*!
            \param from the other Buffer.
         */
        Buffer(Buffer&& from) noexcept;

        //! Deletes the Buffer and attached resources.
        ~Buffer() noexcept;

        //! Move-assigns a Buffer object.
        /*!
            \param from the other Buffer.
         */
        Buffer& operator= (Buffer&& from) noexcept;

        //! Retrieves the underlying Vulkan handle.
        /*!         
            \return the Vulkan Buffer handle.
         */
        inline VkBuffer getHandle() const {
            return _handle;
        }

        //! Implicitly cast to the underlying Vulkan handle.
        inline operator VkBuffer() const noexcept {
            return _handle;
        }

        //! Retrieves the parent Vulkan Device.
        /*!
            \return the device.
         */
        inline Device * getDevice() const {
            return _device;
        }

        //! Retrieves the construction parameter struct.
        /*
            \return the reference to an immutable copy of the parameter struct used to construct this Buffer.
         */
        inline const CreateInfo& getInfo() const {
            return _info;
        }

        //! Maps the Memory object used by this Buffer and returns the memory pointer.
        /*!
            \return the mapped memory pointer.
         */
        void * map();

        //! Unmaps the Memory object used by this Buffer.
        void unmap() noexcept;

        //! Temporarily maps the Memory object and applies a function to it while its mapped.
        /*!
            \param fn the function to apply to the Memory.
         */
        inline void mapping(std::function<void(void *)> fn) {
            fn(map());
            unmap();
        }

        //! Temporarily maps the Memory object and casts it to ptr_t type then applies a function to it while its mapped.
        /*!
            \param fn the function to apply the Memory.
         */
        template<class ptr_t>
        inline void mapping(std::function<void(ptr_t *)> fn) {
            auto pData = reinterpret_cast<ptr_t *> (map());

            fn(pData);
            unmap();
        }

        int getFd() const;
    };
}
