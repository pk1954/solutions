// Int24.ixx 
//
// Toolbox\Utilities
// 
// Handle 24 bit data type 

export module Int24;

import Debug;

export class Int24
{
public:
	Int24()
	{
		m_byte[0] = 0;
		m_byte[1] = 0;
		m_byte[2] = 0;
	}

	Int24(int i)
	{
		Assert(i <= 0xffffff);
		m_byte[0] = i >> 16;
		m_byte[1] = (i & 0x00ff00) >> 8;
		m_byte[2] = (i & 0x0000ff);
	}

	int GetValue() const
	{
		return (m_byte[0] << 16) + (m_byte[1] << 8) + m_byte[2];
	}

protected:
	unsigned char m_byte[3];
};