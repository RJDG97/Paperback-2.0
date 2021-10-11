#pragma once

namespace paperback::editor
{
	//-----------------------------------
	//        Panel Interface
	//-----------------------------------
	struct panel_interface
	{
		void OnSystemCreated(void) noexcept {}
		void Update(void) noexcept {}
	};

	struct instance : panel_interface
	{
		instance() {}
		instance (const instance&) = delete;
		instance& operator=(const instance&) = delete;
	};


	//-----------------------------------
	//          Panel Types
	//-----------------------------------
	namespace type
	{
		using guid = xcore::guid::unit<64, struct system_tag>;

		enum class id : u8
		{
			UPDATE = 0
		};

		enum class call
		{
			CREATED = 0
		,	UPDATE
		};

		struct update
		{
			static constexpr auto id_v = id::UPDATE;

			type::guid                m_Guid{ };
			const char*				  m_pName{ };
		};

		struct info
		{
			using RunSystem  = void( editor::instance&, type::call );
			using Destructor = void( editor::instance& ) noexcept;

			const type::guid             m_Guid;
			const type::id               m_TypeID;
										 
			RunSystem*					 m_RunSystem;
			Destructor*					 m_Destructor;
			const char*					 m_pName{ };
		};
	}

	//-----------------------------------
	//           System Info
	//-----------------------------------
	namespace details
	{
		template< typename T_PANEL >
		editor::type::info CreateInfo(void);

		template< typename T >
		static auto info_v = editor::details::CreateInfo<T>();
	}

	template< typename T_PANEL >
	auto& info_v = details::info_v< std::decay_t<T_PANEL> >;

	namespace details
	{
		template < typename USER_PANEL >
		struct completed final : USER_PANEL
		{
			completed() {};

			void Run(const editor::type::call Type);
		};
	}
}