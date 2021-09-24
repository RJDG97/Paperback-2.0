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

struct Mono_Class{
	MonoClass* mainclass = nullptr;
	MonoClass* testclass = nullptr;

}monoclass;

struct Mono_External_Calls {
	MonoMethod* mono_externaltest = nullptr;

}monoextern;

class Mono 
{
	MonoDomain* MonoDomainptr = nullptr;
	MonoAssembly* MonoAssemblyptr = nullptr;
	MonoImage* MonoImageptr = nullptr;

public:

	uint32_t Mono_Handler_;
	
	// Interface
	MonoObject* mono_obj_;

	Mono()
	{
		// Set Directory for lib and etc
		mono_set_dirs("../../dependencies/mono/lib", "../../dependencies/mono/etc");

		// Create domain (exits if unsuccessful)
		MonoDomainptr = mono_jit_init("Mono");

		// Load binary file as an assembly
		MonoAssemblyptr = mono_domain_assembly_open(MonoDomainptr, "../../build/Paperback_V2/bin/Debug/CSScript.dll");
		if (MonoAssemblyptr) {
			//	 Load mono image
			MonoImageptr = mono_assembly_get_image(MonoAssemblyptr);
			if (MonoImageptr) {
				// Add internal calls (Expose to C# script)
				Mono_Add_Internal_Call();

				// Add classes
				Mono_Add_Classes();

				if (monoclass.mainclass) {
					// Describe Method for main
					MonoMethodDesc* mono_main_desc = mono_method_desc_new(".Main:testinst()", false);
					if (mono_main_desc) {
						MonoMethod* mono_main_method = mono_method_desc_search_in_class(mono_main_desc, monoclass.mainclass);
						if (mono_main_method) {
							// Call main method
							MonoObject* mono_exception = nullptr;
							// Reference object for specified class
							mono_obj_ = thread_callback(mono_main_method, mono_exception);
							if (mono_obj_) {
								// Reference handler for specified class
								Mono_Handler_ = mono_gchandle_new(mono_obj_, false);
							
								// Add External Calls
								Mono_External_Call();
							}
							// Exception Handling
							Mono_Exception(mono_exception);

							// Free Desc
							mono_method_desc_free(mono_main_desc);
						}
					}

				}
			}
		}
	}

	MonoObject* thread_callback(MonoMethod* method, MonoObject* exception)
	{
		mono_jit_thread_attach(MonoDomainptr);
		return mono_runtime_invoke(method, nullptr, nullptr, &exception);
	}

	void Mono_Exception(MonoObject* exception)
	{
		if (exception) {
			MonoString* exString = mono_object_to_string(exception, nullptr);
			const char* exCString = mono_string_to_utf8(exString);

			// Print Error
			std::cout << exCString << std::endl;
			std::cout << std::endl;
		}
	}

	/*************To be Removed **********************/
	static MONO_EXPORT MonoString* internaltest()
	{
		MonoString* str = nullptr;
		const char* func = "Mono Internal Test";
		str = mono_string_new(mono_domain_get(), func);
		return str;
	}

	static MONO_EXPORT void print(MonoString* str)
	{
		std::cout << mono_string_to_utf8(str) << std::endl;
	}

	void externaltest()
	{
		if (monoextern.mono_externaltest)
		{
			MonoObject* exception = nullptr;
			// Get function
			mono_runtime_invoke(monoextern.mono_externaltest, mono_obj_, nullptr, &exception);

			/*
			//Get function (return)
			MonoObject* returnptr_ = mono_runtime_invoke(monoextern.mono_externaltest, mono_obj_, nullptr, &exception);
			// Extract Return result
			if (ptrReturn)
				<return type> result = *(<return type>*)mono_object_unbox(ptrReturn);
			*/

			// Exception Handling
			Mono_Exception(exception);
		}
	}
	/*************To be Removed **********************/

	void Mono_Add_Internal_Call()
	{
		mono_add_internal_call("CSScript.Test::print(string)", &Mono::print);
		mono_add_internal_call("CSScript.Test::internaltest()", &Mono::internaltest);
	}

	void Mono_Add_Classes()
	{
		monoclass.mainclass = mono_class_from_name(MonoImageptr, "CSScript", "Main");
		monoclass.testclass = mono_class_from_name(MonoImageptr, "CSScript", "Test");
	}

	void Mono_External_Call()
	{
		// description of function
		MonoMethodDesc* mono_extern_methoddesc = mono_method_desc_new(".Test:externaltest()", false);
		// get function	
		MonoMethod* vitrualMethod = mono_method_desc_search_in_class(mono_extern_methoddesc, monoclass.testclass);
		if (vitrualMethod)
			monoextern.mono_externaltest = mono_object_get_virtual_method(mono_obj_, vitrualMethod);

		// Free
		mono_method_desc_free(mono_extern_methoddesc);
	}

	~Mono()
	{
		if (Mono_Handler_)
			mono_gchandle_free(Mono_Handler_);

		if (MonoDomainptr)
			mono_jit_cleanup(MonoDomainptr);
	}
};