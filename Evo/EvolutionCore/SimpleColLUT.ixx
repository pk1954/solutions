// SimpleColLUT.ixx
//
// Toolbox\Utilities

export module SimpleColLUT;

export import ColorLUT;

export class SimpleColLUT
{
public:
    SimpleColLUT(Color const colLo, Color const colHi)
    {
        m_colLut.AddBasePoint(ColIndex(0), colLo);
        m_colLut.AddBasePoint(ColIndex(ColorLUT::MAX_INDEX), colHi);
        m_colLut.Construct();
    }

    Color GetColor(ColIndex const index) const 
    {
        return m_colLut.GetColor(index);
    };

    Color GetColorHi() const 
    {
        return GetColor(ColorLUT::MAX_INDEX);
    };

    Color GetColorLo() const 
    {
        return GetColor(ColIndex(0));
    };

    void SetColorHi(Color const col) 
    {
        m_colLut.SetColor(ColorLUT::MAX_INDEX, col);
    };

    void SetColorLo(Color const col) 
    {
        m_colLut.SetColor(ColIndex(0), col);
    };

private:
    ColorLUT m_colLut;
};