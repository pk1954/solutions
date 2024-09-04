// HistCacheItem.ixx 
//
// History

export module HistoryLib:HistCacheItem;

import std;
import :GenerationCmd;
import :HistGeneration;
import :ModelData;

using std::unique_ptr;

export class HistCacheItem
{
public:

    HistCacheItem(ModelFactory const &factory) :
        m_genHistCounter(0L),
        m_genCmd()
    {
        m_upModelData = factory.CreateModelData();
    }

    virtual ~HistCacheItem() {};

    virtual void CopyCacheItemFrom(HistCacheItem const* const pSrc)
    {
        // std::wcout << __FUNCTION__ << L" Gen" << pSrc->m_genHistCounter << L" ->" << m_genHistCounter << endl;
        m_upModelData->CopyFrom(*(pSrc->m_upModelData.get()));
        m_genHistCounter = pSrc->m_genHistCounter;
        m_genCmd         = pSrc->m_genCmd;
    }

    ModelData      * GetModelData()            { return m_upModelData.get(); }
    ModelData const* GetModelDataC()     const { return m_upModelData.get(); }
    HistGeneration   GetHistGenCounter() const { return m_genHistCounter; }
    GenerationCmd    GetGenCmd()         const { return m_genCmd; }
    size_t           GetItemSize()       const { return m_upModelData->GetModelSize() + sizeof(HistCacheItem); }

    void ZeroHistGenCounter() { m_genHistCounter = 0L; }
    void IncHistGenCounter()  { ++m_genHistCounter; }

    void InitializeGenCmd() { m_genCmd.InitializeCmd(); }
    void SetGenerationCommand(GenerationCmd cmd) { m_genCmd = cmd; }

private:
    unique_ptr<ModelData> m_upModelData;

    HistGeneration m_genHistCounter;
    GenerationCmd  m_genCmd;
};
