#pragma once
 
#ifndef MONO
#define MONO_EXPORT __declspec(dllexport)
#else
#define MONO_EXPORT __declspec(dllimport)
#endif

#include <iostream>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include <mono/metadata/appdomain.h>

#include "Scripts/MonoInternal.h"
#include "Scripts/MonoExternal.h"

struct Mono_Class{
	MonoClass* m_pMain = nullptr;
}monoclass;

struct Mono_External_Calls {
	MonoMethod* m_pMain = nullptr;
}monoextern;

class Mono 
{
	MonoDomain* m_pMonoDomain = nullptr;
	MonoAssembly* m_pMonoAssembly = nullptr;
	MonoImage* m_pMonoImage = nullptr;

public:

	uint32_t m_MonoHandler;
	// Interface
	MonoObject* m_pMonoObj;

	Mono()
	{
		// Set Directory for lib and etc
		mono_set_dirs("../../dependencies/mono/lib", "../../dependencies/mono/etc");

		// Create domain (exits if unsuccessful)
		m_pMonoDomain = mono_jit_init("Mono");

		// Load binary file as an assembly
		m_pMonoAssembly = mono_domain_assembly_open(m_pMonoDomain, "../../build/Paperback_V2/bin/Debug/CSScript.dll");
		if (m_pMonoAssembly) {
			//	 Load mono image
			m_pMonoImage = mono_assembly_get_image(m_pMonoAssembly);
			if (m_pMonoImage) {
				// Add internal calls (Expose to C# script)
				MONO_INTERNALS::MonoAddInternalCall();
				
				// Add classes
				MonoAddClasses();

				if (monoclass.m_pMain) {
					// Describe Method for main
					MonoMethodDesc* mono_main_desc = mono_method_desc_new(".Main:getInst()", false);
					if (mono_main_desc) {
						MonoMethod* mono_main_method = mono_method_desc_search_in_class(mono_main_desc, monoclass.m_pMain);
						if (mono_main_method) {
							// Call main method
							MonoObject* mono_exception = nullptr;
							// Reference object for specified class
							m_pMonoObj = ThreadCallback(mono_main_method, mono_exception);
							if (m_pMonoObj) {
								// Reference handler for specified class
								m_MonoHandler = mono_gchandle_new(m_pMonoObj, false);
							
								// Add External Calls
								MonoExternalCall();
							}
							// Exception Handling
							MonoException(mono_exception);

							// Free Desc
							mono_method_desc_free(mono_main_desc);
						}
					}

				}
			}
		}
	}

	MonoObject* ThreadCallback(MonoMethod* method, MonoObject* exception)
	{
		mono_jit_thread_attach(m_pMonoDomain);
		return mono_runtime_invoke(method, nullptr, nullptr, &exception);
	}

	void MonoException(MonoObject* exception)
	{
		if (exception) {
			MonoString* exString = mono_object_to_string(exception, nullptr);
			const char* exCString = mono_string_to_utf8(exString);

			// Print Error
			std::cout << exCString << std::endl;
			std::cout << std::endl;
		}
	}

	void ExternalMain()
	{
		if (monoextern.m_pMain)
		{
			MonoObject* exception = nullptr;
			// Get function
			mono_runtime_invoke(monoextern.m_pMain, m_pMonoObj, nullptr, &exception);

			/*
			//Get function (return)
			MonoObject* returnptr_ = mono_runtime_invoke(monoextern.mono_externaltest, mono_obj_, nullptr, &exception);
			// Extract Return result
			if (ptrReturn)
				<return type> result = *(<return type>*)mono_object_unbox(ptrReturn);
			*/

			// Exception Handling
			MonoException(exception);
		}
	}

	void MonoAddClasses()
	{
		monoclass.m_pMain = mono_class_from_name(m_pMonoImage, "CSScript", "Main");
	}

	void MonoExternalCall()
	{
		// description of function
		MonoMethodDesc* mono_extern_methoddesc = mono_method_desc_new(".Main:main()", false);
		// get function	
		MonoMethod* vitrualMethod = mono_method_desc_search_in_class(mono_extern_methoddesc, monoclass.m_pMain);
		if (vitrualMethod)
			monoextern.m_pMain = mono_object_get_virtual_method(m_pMonoObj, vitrualMethod);
		// Free
		mono_method_desc_free(mono_extern_methoddesc);
	}

	void ReleaseDomain()
	{
		if (m_MonoHandler)
			mono_gchandle_free(m_MonoHandler);

		if (m_pMonoDomain)
			mono_jit_cleanup(m_pMonoDomain);
	}

	static Mono& GetInstanced()
	{
		static Mono mono;
		return mono;
	}
};