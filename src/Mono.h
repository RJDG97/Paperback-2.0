/**********************************************************************************
*\file         Mono.h
*\brief        Mono.h
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#ifndef MONO
#define MONO_EXPORT __declspec(dllexport)
#else
#define MONO_EXPORT __declspec(dllimport)
#endif

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include <mono/metadata/appdomain.h>

#include "paperback_logger.h"
#include "Scripts/MonoInternal.h"

class Mono
{
	MonoDomain* m_pMonoDomain = nullptr;
	MonoDomain* m_pMonoDomain2 = nullptr;
	MonoAssembly* m_pMonoAssembly = nullptr;
	MonoImage* m_pMonoImage = nullptr;

	bool m_UsingDomain1 = true;

public:

	uint32_t m_MonoHandler;

	MonoObject* m_pMainObj;
	MonoClass* m_pMainClass = nullptr;
	MonoMethod* m_pMainFn = nullptr;

	Mono()
	{
		// Set Directory for lib and etc
		mono_set_dirs("Dependencies/mono/lib", "Dependencies/mono/etc");

		// Create domain (exits if unsuccessful)
		m_pMonoDomain = mono_jit_init("Mono");

		if (LoadAssembly(m_pMonoDomain, "Dependencies/Scriptlib.dll")) {
			// Add internal calls (Expose to C# script)
			MONO_INTERNALS::MonoAddInternalCall();

			// Add classes
			m_pMainClass = ImportClass("CSScript", "MainApplication");

			if (m_pMainClass) {
				// Describe Method for main
				m_pMainObj = GetClassInstance(".MainApplication:getInst()", m_pMainClass);

				if (m_pMainObj) {
					// Reference handler for specified class
					m_MonoHandler = mono_gchandle_new(m_pMainObj, false);

					// Add External Calls
					m_pMainFn = ImportFunction(m_pMainClass, m_pMainObj, ".MainApplication:Main()");
				}
			}
		}
	}

	bool UpdateDLL()
	{
		char m_DName[] = "Update";

		if (m_UsingDomain1) {
			std::system("mcs_ScriptRuntimelib.bat");
			m_pMonoDomain2 = mono_domain_create_appdomain(m_DName, NULL);
			mono_domain_set(m_pMonoDomain2, false);
			if (LoadAssembly(m_pMonoDomain2, "Dependencies/ScriptRuntimelib.dll"))
			{
				// Unload Previous Domain
				UnloadDomain(m_pMonoDomain);
				m_UsingDomain1 = false;
				return true;
			}
			else
				UnloadDomain(m_pMonoDomain2);

		}
		else {
			m_pMonoDomain = mono_domain_create_appdomain(m_DName, NULL);
			mono_domain_set(m_pMonoDomain, false);
			if (LoadAssembly(m_pMonoDomain, "Dependencies/Scriptlib.dll"))
			{
				// Unload Previous Domain
				UnloadDomain(m_pMonoDomain2);
				m_UsingDomain1 = true;
				return UpdateDLL();
			}
			else
				UnloadDomain(m_pMonoDomain);
		}
		return false;
	}

	bool LoadAssembly(MonoDomain* Domain, const char* dllname)
	{
		if (Domain) {	// load assembly 			
			m_pMonoAssembly = mono_domain_assembly_open(Domain, dllname);
			if (m_pMonoAssembly) {	//	 Load mono image
				m_pMonoImage = mono_assembly_get_image(m_pMonoAssembly);
				if (m_pMonoImage)
					return true;
				else
					PPB_ASSERT("Mono Image not initialized");
			}
			PPB_ASSERT("Mono Assembly not initialized");
		}
		PPB_ASSERT("Mono Domain not created");
		return false;
	}

	void UnloadDomain(MonoDomain* domainToUnload)
	{
		if (domainToUnload && domainToUnload != mono_get_root_domain())
		{
			mono_domain_set(mono_get_root_domain(), false);
			mono_domain_unload(domainToUnload);
		}
	}

	MonoObject* GetClassInstance(const char* m_pFnDesc, MonoClass* m_pClass)
	{
		MonoObject* m_pMonoObj = nullptr;

		MonoMethodDesc* mono_main_desc = mono_method_desc_new(m_pFnDesc, false);
		if (mono_main_desc) {
			MonoMethod* mono_main_method = mono_method_desc_search_in_class(mono_main_desc, m_pClass);
			if (mono_main_method) {
				// Call main method
				MonoObject* mono_exception = nullptr;
				// Reference object for specified class
				m_pMonoObj = mono_runtime_invoke(mono_main_method, nullptr, nullptr, &mono_exception);
				// Exception Handling
				MonoException(mono_exception);
			}
			// Free Desc
			mono_method_desc_free(mono_main_desc);
		}
		if (m_pMonoObj == nullptr) {
			std::string str = m_pFnDesc;
			WARN_PRINT(str + " function not found");
		}

		return m_pMonoObj;
	}

	MonoObject* ThreadCallback(MonoObject* m_pObj, MonoMethod* exception)
	{
		//mono_jit_thread_attach(m_pMonoDomain);
		//return mono_runtime_invoke(m_pObj, nullptr, nullptr, &exception);
	}

	void MonoException(MonoObject* exception)
	{
		if (exception) {
			MonoString* exString = mono_object_to_string(exception, nullptr);
			const char* exCString = mono_string_to_utf8(exString);

			// Print Error
			CRITICAL_LOG(exCString);
			CRITICAL_PRINT(exCString);
			PPB_ASSERT(exCString);
		}
	}

	MonoObject* RunImportFn(MonoObject* m_pObj, MonoMethod* m_pFn)
	{
		MonoObject* fn = nullptr;
		if (m_pFn)
		{
			MonoObject* exception = nullptr;
			mono_jit_thread_attach(m_pMonoDomain);
			// Get function
			fn = mono_runtime_invoke(m_pFn, m_pObj, nullptr, &exception);
			if (exception)	// Exception Handling
				MonoException(exception);
		}
		return fn;
	}

	template <typename ...Args>
	MonoObject* RunImportFn(MonoObject* m_pObj, MonoMethod* m_pFn, Args... args)
	{
		// Unpack Arguments
		void* m_Arguments[] = { &args... };
		// Run Function
		MonoObject* fn = nullptr;
		if (m_pFn && m_pObj)
		{
			MonoObject* exception = nullptr;
			mono_jit_thread_attach(m_pMonoDomain);
			if (m_Arguments)	// Get function w arguments
				fn = mono_runtime_invoke(m_pFn, m_pObj, m_Arguments, &exception);
			else	// Get function without params
				fn = mono_runtime_invoke(m_pFn, m_pObj, nullptr, &exception);

			if (exception)
			{
				// Exception Handling
				MonoException(exception);
			}
		}
		return fn;
	}

	template <typename T>
	T ExtractResult(MonoObject* m_pResult)
	{
		return *(T*)mono_object_unbox(m_pResult);
	}

	MonoClass* ImportClass(const char* _namespace, const char* _class)
	{
		if (m_pMonoImage == NULL) {
			PPB_ASSERT("Mono Image not initialized");
			return nullptr;
		}
		
		MonoClass* result = mono_class_from_name(m_pMonoImage, _namespace, _class);
		if (!result) {
			std::string str = _class;
			WARN_PRINT(str + " class not found");
		}
		return result;
	}

	MonoMethod* ImportFunction(MonoClass* m_pClass, MonoObject* m_pObj, const char* m_pFnDesc)
	{
		MonoMethod* fn = nullptr;
		// description of Start function
		MonoMethodDesc* mono_extern_methoddesc = mono_method_desc_new(m_pFnDesc, false);
		MonoMethod* vitrualMethod = mono_method_desc_search_in_class(mono_extern_methoddesc, m_pClass);

		if (vitrualMethod)
			fn = mono_object_get_virtual_method(m_pObj, vitrualMethod);
		else {
			std::string str = m_pFnDesc;
			WARN_PRINT(str + " function not found");
		}

		// Free
		mono_method_desc_free(mono_extern_methoddesc);

		return fn;
	}

	void ReleaseDomain()
	{
		if (m_MonoHandler)
			mono_gchandle_free(m_MonoHandler);
		if (m_UsingDomain1)
			mono_jit_cleanup(m_pMonoDomain);
		else
			mono_jit_cleanup(m_pMonoDomain2);
	}


	static Mono& GetInstanced()
	{
		static Mono mono;
		return mono;
	}
};