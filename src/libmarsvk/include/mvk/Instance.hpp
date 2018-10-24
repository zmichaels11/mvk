#pragma once

#include <cstddef>

#include "volk.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "mvk/InstanceLayer.hpp"
#include "mvk/PhysicalDevice.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    //! Singleton wrapper object that represents the interface to the Vulkan drivers.
    class Instance {
        VkInstance _handle;
        std::vector<PhysicalDevice> _physicalDevices;

        Instance();

        ~Instance();

        Instance(const Instance&) = delete;

        Instance& operator= (const Instance&) = delete;

        public:
            Instance(Instance&& from) noexcept:
                _handle(std::exchange(from._handle, nullptr)) {}

            Instance& operator= (Instance&& from) noexcept;

            //! Enables a Vulkan Instance Layer by name.
            /*!
                Instance layers are only enabled when built as DEBUG.
                The application may terminate if a layer is not found on the system PATH.
                This method is only valid if the Instance has not yet been initialized.
                \param layer the name of the Instance layer.
            */
            static void enableLayer(const std::string& layer) noexcept;

            //! Enables an InstanceLayer known at compile-time
            /*!
                Instance layers are only enabled when built as DEBUG.
                The application may terminate if a layer is not found on the system PATH.
                This method is only valid if the Instance has not yet been initialized.
                \param the InstanceLayer.
            */
            inline static void enableLayer(const InstanceLayer& layer) noexcept {
                enableLayer(std::to_string(layer));
            }

            //! Enables all extensions required by GLFW
            /*!
                The extensions required by GLFW are system dependent.
                The application may terminate if an extension is not available.
                This method is only valid if the Instance has not yet been initialized.
            */
            static void enableRequiredGLFWExtensions() noexcept;

            //! Enables an extension by name.
            /*!
                The application may terminate if an extension is not available.
                This method is only valid if the Instance has not yet been initialized.
                \param extension the name of the extension.
            */
            static void enableExtension(const std::string& extension) noexcept;

            //! Retrieves the current Instance.
            /*!
                This will initialize the Instance if it has not yet been initialized.
                Future calls to enableLayer and enableExtension will be invalid.
                \return the Instance.
            */
            static Instance& getCurrent();

            //! Explicitly frees the Instance.
            /*!
                The Instance is otherwise implicitly freed once the application terminates.
            */
            void free() noexcept;

            //! Retrieves the underlying Vulkan Instance handle.
            /*!
                \return the handle
            */
            inline VkInstance getHandle() const noexcept {
                return _handle;
            }

            //! Retrieves the number of Vulkan devices.
            /*!
                A PhysicalDevice generally maps to a Vulkan-capable GPU.
                \return the number of Vulkan devices.
            */
            inline std::size_t getPhysicalDeviceCount() const noexcept {
                return _physicalDevices.size();
            }

            //! Retrieves the specified PhysicalDevice.
            /*!
                \param index is the index of the PhysicalDevice.
                \return PhysicalDevice the PhysicalDevice.
            */
            inline PhysicalDevice& getPhysicalDevice(std::ptrdiff_t index) noexcept {
                return _physicalDevices[index];
            }

            //! Retrieves a list of all available PhysicalDevices.
            /*!
                \return the list of PhysicalDevices.
            */
            inline const std::vector<PhysicalDevice>& getPhysicalDevices() const noexcept {
                return _physicalDevices;
            }

            //! Implicitly casts the Instance to a Vulkan Instance handle.
            inline operator VkInstance() const noexcept {
                return _handle;
            }
    };
}
