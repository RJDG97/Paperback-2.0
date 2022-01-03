#pragma once

#include "Mono.h"
#include "paperback_coordinator.h"
#include "paperback_logger.h"

class Script
{
	// Mono Class Pointer
	MonoClass* m_pClass = nullptr;
	// Mono Object Instance
	MonoObject* m_pMonoObj = nullptr;
	// Mono Function Pointers
	MonoMethod* m_pStart = nullptr;
	MonoMethod* m_pUpdate = nullptr;
	MonoMethod* m_pDestroy = nullptr;
	// Class Name
	std::string m_ScriptClass;

public:

	void Init(std::string m_class)
	{
		m_ScriptClass = m_class;
		ScriptUpdate();
	}

	void ScriptUpdate()
	{
		std::string str;
		// Assign Class 
		m_pClass = Mono::GetInstanced().ImportClass("CSScript", m_ScriptClass.c_str());

		if (m_pClass) {

			str = "." + m_ScriptClass + ":getInst()";
			m_pMonoObj = Mono::GetInstanced().GetClassInstance(str.c_str(), m_pClass);

			if (m_pMonoObj) {

				str = "." + m_ScriptClass + ":Start(uint)";
				m_pStart = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":Update(single)";
				m_pUpdate = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":Destroy()";
				m_pDestroy = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());
			}
		}
	}

	void Start(uint32_t ID)
	{
		if (m_pStart)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pStart, ID);
	}

	void Update(float dt)
	{
		if (m_pUpdate)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pUpdate, dt);
	}

	void Destroy()
	{
		if (m_pDestroy)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pDestroy);
	}
};
