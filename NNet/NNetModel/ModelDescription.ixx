// ModelDescription.ixx
//
// NNetModel

module;

#include <vector>
#include <string>

export module NNetModel:ModelDescription;

import DescriptionUI;

using std::vector;
using std::wstring;

export class ModelDescription
{
public:
	void SetDescriptionUI(DescriptionUI& descUI)
	{
		m_pDescUI = &descUI;
		push2Editor();
	}

	void ClearDescription()
	{
		m_lines.clear();
		m_pDescUI->ClearDescription();
		m_pDescUI->ResetDirtyFlag();
	}

	void AddDescriptionLine(wstring const wstrLine)
	{
		m_lines.push_back(wstrLine);
	}

	void DescriptionComplete()
	{
		if (m_pDescUI)
			push2Editor();
	}

	bool GetDescriptionLine(unsigned int const uiLineNr, wstring& wstrLine) const
	{
		if (m_pDescUI && m_pDescUI->IsDirty())
			reloadFromEditor();

		if (uiLineNr >= m_lines.size())
			return false;

		wstrLine = m_lines[uiLineNr];
		return true;
	}

private:
	DescriptionUI * m_pDescUI{ nullptr };
	mutable vector<wstring> m_lines;

	void reloadFromEditor() const
	{
		m_lines.resize(m_pDescUI->GetLineCount());
		for (unsigned int uiLineNr = 0; uiLineNr < m_lines.size(); ++uiLineNr)
			m_pDescUI->GetDescriptionLine(uiLineNr, m_lines[uiLineNr]);
		m_pDescUI->ResetDirtyFlag();
	}

	void push2Editor()
	{
		wstring wstrBuffer{ L"" };
		for (wstring& wstrLine : m_lines)
			wstrBuffer += wstrLine + L"\r\n";
		m_pDescUI->SetDescription(wstrBuffer);
		m_pDescUI->ResetDirtyFlag();
	}
};
