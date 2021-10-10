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

		instance( void )			noexcept = default;
		instance( const instance& ) noexcept = delete;

		~instance( void ) noexcept;

		PPB_INLINE
		void Init( std::span<const component::info* const> Types, const u32 NumComponents ) noexcept;

		PPB_INLINE
		u32 Append( void ) noexcept;

		PPB_INLINE
		u32 Delete( const u32 PoolIndex ) noexcept;

		PPB_INLINE
		void Clear( void ) noexcept;

		PPB_INLINE
		bool RemoveTransferredEntity( const u32 PoolIndex ) noexcept;

		PPB_INLINE
		u32 TransferExistingComponents( const PoolDetails& Details, vm::instance& Pool ) noexcept;

		PPB_INLINE
		void CloneComponents( const u32 ToIndex, const u32 FromIndex ) noexcept;

		template < typename T_COMPONENT >
		T_COMPONENT& GetComponent( const u32 PoolIndex ) const noexcept;

		PPB_INLINE
		int GetComponentIndex( const u32 UIDComponent ) const noexcept;

		PPB_INLINE
		int GetComponentIndexFromGUID( const component::type::guid Guid ) const noexcept;
		
		PPB_INLINE
		int GetComponentIndexFromGUIDInSequence( const component::type::guid Guid, const int Sequence ) const noexcept;

		PPB_INLINE
		void SerializePoolComponentsAtEntityIndex( const u32 Index, paperback::JsonFile& Jfile ) noexcept;
		
		PPB_INLINE
		std::vector<rttr::instance> GetComponents( const u32 Index ) noexcept;

		PPB_INLINE
		rttr::instance GetComponentInstance( const component::type::guid Comp_Guid, const u32 Index ) noexcept;

		PPB_INLINE
		u32 GetCurrentEntityCount( void ) const noexcept;

		PPB_INLINE
		paperback::vm::instance::MemoryPool& GetMemoryPool( void ) noexcept;


	private:

		PPB_INLINE
		u32 GetPageIndex( const size_t LocalComponentIndex, const u32 Count ) const noexcept;

		PPB_INLINE
		u32 GetPageIndex( const component::info& Info, const u32 Count ) const noexcept;


		std::span<const component::info* const>			m_ComponentInfo				{   };
		paperback::vm::instance::MemoryPool				m_MemoryPool				{   };
		u32						        				m_NumberOfComponents		{ 0 };
		u32						        				m_CurrentEntityCount		{ 0 };
	};
}