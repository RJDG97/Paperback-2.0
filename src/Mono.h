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

#include "Scripts/MonoInternal.h"

class Mono 
{
	MonoDomain* m_pMonoDomain = nullptr;
	MonoAssembly* m_pMonoAssembly = nullptr;
	MonoImage* m_pMonoImage = nullptr;

public:

	uint32_t m_MonoHandler;

	MonoObject* m_pMainObj;
	MonoClass* m_pMainClass = nullptr;
	MonoMethod* m_pMainFn = nullptr;

	Mono()
	{
		// Set Directory for lib and etc
		mono_set_dirs("../../dependencies/mono/lib", "../../dependencies/mono/etc");

		// Create domain (exits if unsuccessful)
		m_pMonoDomain = mono_jit_init("Mono");

		if(UpdateDLL()){
			// Add internal calls (Expose to C# script)
			MONO_INTERNALS::MonoAddInternalCall();
			
			// Add classes
			m_pMainClass = ImportClass("CSScript", "Main");

			if (m_pMainClass) {
				// Describe Method for main
				m_pMainObj = GetClassInstance(".Main:getInst()", m_pMainClass);

				if (m_pMainObj) {
					// Reference handler for specified class
					m_MonoHandler = mono_gchandle_new(m_pMainObj, false);
				
					// Add External Calls
					m_pMainFn = ImportFunction(m_pMainClass, m_pMainObj, ".Main:main()");
				}
			}
		}
	}

	bool UpdateDLL()
	{
		// Load binary file as an assembly
		m_pMonoAssembly = mono_domain_assembly_open(m_pMonoDomain, "../../build/Paperback_V2/bin/Debug/CSScript.dll");
		if (m_pMonoAssembly) {
			//	 Load mono image
			m_pMonoImage = mono_assembly_get_image(m_pMonoAssembly);
			if (m_pMonoImage)
				return true;
		}
		return false;
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
				m_pMonoObj = ThreadCallback(mono_main_method, mono_exception);
				// Exception Handling
				MonoException(mono_exception);
			}
			// Free Desc
			mono_method_desc_free(mono_main_desc);
		}
		return m_pMonoObj;
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
			PPB_ASSERT(exCString);
		}
	}

	MonoObject* RunImportFn(MonoObject* m_pObj, MonoMethod* m_pFn)
	{
		if (m_pFn)
		{
			MonoObject* exception = nullptr;
			// Get function
			return mono_runtime_invoke(m_pFn, m_pObj, nullptr, &exception);
			
			// Exception Handling
			MonoException(exception);
		}
		return nullptr;
	}

	template <typename T>
	T ExtractResult(MonoObject* m_pResult)
	{
		return *(T*)mono_object_unbox(m_pResult);
	}

	MonoClass* ImportClass(const char* _namespace, const char* _class)
	{
		return mono_class_from_name(m_pMonoImage, _namespace, _class);
	}

	MonoMethod* ImportFunction(MonoClass* m_pClass, MonoObject* m_pObj, const char* m_pFnDesc)
	{
		MonoMethod* fn = nullptr;
		// description of Start function
		MonoMethodDesc* mono_extern_methoddesc = mono_method_desc_new(m_pFnDesc, false);
		MonoMethod* vitrualMethod = mono_method_desc_search_in_class(mono_extern_methoddesc, m_pClass);

		if (vitrualMethod)
			fn = mono_object_get_virtual_method(m_pObj, vitrualMethod);

		// Free
		mono_method_desc_free(mono_extern_methoddesc);

		return fn;
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