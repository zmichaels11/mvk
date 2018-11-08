#pragma once

#include <utility>

namespace mvk {
    class Fence;
    
    //! A wrapper for a Fence object that will release when it exits scope.
    class ManagedFence {
        Fence * _pFence;

        ManagedFence(const ManagedFence&) = delete;
        ManagedFence& operator= (const ManagedFence&) = delete;

    public:
        //! Creates an empty ManagedFence.
        /*!
            The initial object state holds nullptr and will do nothing when it exits scope.
            It can be assigned a raw Fence pointer. At that point, it will manage that Fence.
        */
        ManagedFence() noexcept:
            _pFence(nullptr) {}

        //! Creates a ManagedFence by wrapping an existing Fence.
        /*!
            The Fence object will be released when the ManagedFence exits scope or a
            new Fence is assigned.

            \param pFence is the Fence.
        */
        ManagedFence(Fence * pFence) noexcept:
            _pFence(pFence) {}

        //! Move-constructs the MangedFence.
        /*!
            This will steal the Fence from the other ManagedFence.
            The other MangedFence will afterwards hold nullptr.

            \param from the other ManagedFence.
        */
        ManagedFence(ManagedFence&& from) noexcept:
            _pFence(std::exchange(from._pFence, nullptr)) {}
        
        //! Deletes the ManagedFence and releases any held Fence if it is not null.
        ~ManagedFence() noexcept;

        //! Move-assignes the ManagedFence.
        /*!
            \param from the other ManagedFence.
        */
        ManagedFence& operator= (ManagedFence&& from) noexcept;

        //! Assigns a Fence to be owned by this ManagedFence.
        /*!
            Any Fence object held by this ManagedFence prior to this operation will be released.

            \param pFence the Fence.
        */
        ManagedFence& operator= (Fence * pFence) noexcept;

        //! Retrieves the pointer to the Fence object.
        /*!
            \return the pointer to the Fence.
        */
        inline Fence * get() const noexcept {
            return _pFence;
        }

        //! Forwards a function operation to the managed Fence object.
        inline Fence * operator->() const noexcept {
            return _pFence;
        }

        inline operator Fence*() const noexcept {
            return _pFence;
        }
    };
}
