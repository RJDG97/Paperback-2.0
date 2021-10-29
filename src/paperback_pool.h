#pragma once

namespace paperback::vm
{
	struct PoolDetails
	{
		u32 m_Key			= settings::invalid_index_v;	// Access Pool Within Archetype
		u32 m_PoolIndex		= settings::invalid_index_v;	// Access Component Within Pool
	};

	class instance final
	{
	public:

		using MemoryPool = std::array<std::byte*, paperback::settings::max_components_per_entity_v>;

		//-----------------------------------
		//            Default
		//-----------------------------------

		PPB_INLINE
		instance( void )			noexcept = default;

		PPB_INLINE
		instance( const instance& ) noexcept = delete;

		PPB_INLINE
		~instance( void ) noexcept;

		PPB_INLINE
		void Init( std::span<const component::info* const> Types, const u32 NumComponents, paperback::coordinator::instance* Coordinator ) noexcept;


		//-----------------------------------
		//         Create / Delete
		//-----------------------------------

		PPB_INLINE
		u32 Append( void ) noexcept;

		PPB_INLINE // Marks entity to be deleted ( Does not perform actual deletion )
		void DestroyEntity( paperback::component::entity& Entity ) noexcept;

		PPB_INLINE // Performs actual deletion
		u32 Delete( const u32 PoolIndex ) noexcept;

		PPB_INLINE
		void Clear( void ) noexcept;

		PPB_INLINE
		bool RemoveTransferredEntity( const u32 PoolIndex ) noexcept;

		PPB_INLINE
		void UpdateStructuralChanges( void ) noexcept;


		//-----------------------------------
		//             Transfer
		//-----------------------------------

		PPB_INLINE
		u32 TransferExistingComponents( const PoolDetails& Details, vm::instance& Pool ) noexcept;


		//-----------------------------------
		//              Clone
		//-----------------------------------

		PPB_INLINE
		void CloneComponents( const u32 ToIndex, const u32 FromIndex ) noexcept;


		//-----------------------------------
		//              Save
		//-----------------------------------

		PPB_INLINE
		void SerializePoolComponentsAtEntityIndex( const u32 Index, paperback::JsonFile& Jfile ) noexcept;


		//-----------------------------------
		//            Getters
		//-----------------------------------

		template < typename T_COMPONENT >
		T_COMPONENT* FindComponent( const u32 PoolIndex ) const noexcept;

		template < typename T_COMPONENT >
		T_COMPONENT& GetComponent( const u32 PoolIndex ) const noexcept;

		PPB_INLINE
		int GetComponentIndex( const u32 UIDComponent ) const noexcept;

		PPB_INLINE
		int GetComponentIndexFromGUID( const component::type::guid Guid ) const noexcept;
		
		PPB_INLINE
		int GetComponentIndexFromGUIDInSequence( const component::type::guid Guid, const int Sequence ) const noexcept;
		
		PPB_INLINE
		std::vector<rttr::instance> GetComponents( const u32 Index ) noexcept;

		PPB_INLINE
		rttr::instance GetComponentInstance( const component::type::guid Comp_Guid, const u32 Index ) noexcept;

		PPB_INLINE
		const u32 GetCurrentEntityCount( void ) const noexcept;

		PPB_INLINE
		const u32 GetComponentCount( void ) const noexcept;

		PPB_INLINE
		paperback::vm::instance::MemoryPool& GetMemoryPool( void ) noexcept;


	private:

		PPB_INLINE
		u32 GetPageIndex( const size_t LocalComponentIndex, const u32 Count ) const noexcept;

		PPB_INLINE
		u32 GetPageIndex( const component::info& Info, const u32 Count ) const noexcept;

		PPB_INLINE
		void MarkEntityAsMoved( const u32 MovedEntity ) noexcept;


		paperback::coordinator::instance*				m_pCoordinator;
		paperback::vm::instance::MemoryPool				m_MemoryPool				{   };
		std::span<const component::info* const>			m_ComponentInfo				{   };
		u32						        				m_NumberOfComponents		{ 0 };
		u32						        				m_CurrentEntityCount		{ 0 };
        u32												m_MoveHead                  { settings::invalid_delete_index_v };
        u32												m_DeleteHead                { settings::invalid_delete_index_v };
	};
}