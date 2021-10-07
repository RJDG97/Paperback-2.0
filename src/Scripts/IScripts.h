#pragma once

#include "Mono.h"

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

public:

	Script(std::string m_class)
	{
		std::string str;
		// Assign Class 
		m_pClass = Mono::GetInstanced().ImportClass("CSScript", m_class.c_str());

		if (m_pClass) {
			str = "." + m_class + ":getInst()";
			m_pMonoObj = Mono::GetInstanced().GetClassInstance(str.c_str(), m_pClass);

			if (m_pMonoObj)
			{
				str = "." + m_class + ":Start()";
				m_pStart = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());
				str = "." + m_class + ":Update()";
				m_pUpdate = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());
				str = "." + m_class + ":Destroy()";
				m_pDestroy = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());
			}
		}
	}

	void Start()
	{
		if (m_pStart)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pStart);
	}

	void Update()
	{
		if (m_pUpdate)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pUpdate);
	}

	void Destroy()
	{
		if(m_pDestroy)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pDestroy);
	}

	~Script()
	{
		if(m_pClass) delete m_pClass;
		if (m_pMonoObj) delete m_pMonoObj;
		if (m_pStart) delete m_pStart;
		if (m_pUpdate) delete m_pUpdate;
		if (m_pDestroy) delete m_pDestroy;
	}
};
