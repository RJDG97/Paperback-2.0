#pragma once

#include "Mono.h"

class IScript
{
public:
	// Mono Class Pointer
	MonoClass* m_pClass = nullptr;
	// Mono Object Instance
	MonoObject* m_pMonoObj = nullptr;
	uint32_t m_MonoHandler;
	// Mono Function Pointers
	MonoMethod* m_pStart = nullptr;
	MonoMethod* m_pUpdate = nullptr;
	MonoMethod* m_pDestroy = nullptr;

	virtual void Start();

	virtual void Update();

	virtual void Destroy();

	virtual ~IScript();
};

class Unit : protected IScript
{
	Unit()
	{
		// Assign Class 
		m_pClass = mono_class_from_name(Mono::GetInstanced().m_pMonoImage, "CSScript", "Unit");

		if (m_pClass) {
			// Describe Method for Class Instance
			MonoMethodDesc* m_pInstanceDesc = mono_method_desc_new(".Unit:GetInstance()", false);

			if (m_pInstanceDesc) {
				// Search class for  Instance function
				MonoMethod* m_pInstancemethod = mono_method_desc_search_in_class(m_pInstanceDesc, m_pClass);
				
				if (m_pInstancemethod) {

					// Call main method
					MonoObject* mono_exception = nullptr;

					// Reference object for specified class
					m_pMonoObj = Mono::GetInstanced().ThreadCallback(m_pInstancemethod, mono_exception);
					
					if (m_pMonoObj) {
						// Reference handler for specified class
						m_MonoHandler = mono_gchandle_new(m_pMonoObj, false);

						// Add External Calls
						RegisterFunction(".Unit:Start()", m_pStart);
						RegisterFunction(".Unit:Update()", m_pUpdate);
						RegisterFunction(".Unit:Destroy()", m_pDestroy);
					}
					// Exception Handling
					MonoException(mono_exception);

					// Free Desc
					mono_method_desc_free(m_pInstanceDesc);
				}
			}
		}
	}

	void RegisterFunction(std::string fn, MonoMethod* fnptr)
	{
		// description of Start function
		MonoMethodDesc* mono_extern_methoddesc; = mono_method_desc_new(fn, false);
		MonoMethod* vitrualMethod; = mono_method_desc_search_in_class(mono_extern_methoddesc, m_pClass);

		if (vitrualMethod)
			fnptr = mono_object_get_virtual_method(m_pMonoObj, vitrualMethod);

		// Free
		mono_method_desc_free(mono_extern_methoddesc);
	}

	void Start()
	{
	
	}

	void Update()
	{
	
	}

	void Destroy()
	{
	
	}
};
