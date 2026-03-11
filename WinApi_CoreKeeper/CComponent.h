#pragma once
class CComponent
{
private:
	CComponent();
	CComponent(const CComponent& rhs) = delete;
	CComponent& operator=(CComponent& rObjMgr) = delete;
	~CComponent();

public:
	static CComponent* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CComponent;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CComponent* m_pInstance;
};