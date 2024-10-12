// Animation.ixx
//
// Win32_utilities

export module Animation;

import std;
import WinBasics;
import SmoothMoveFp;
import ThreadPoolTimer;

using std::move;
using std::bit_cast;
using std::function;
//using std::same_as;

DWORD const ANIMATION_RECURRING { 0x1L };

//template <typename T>
//concept IsLinearCombinable = requires(T a, T b, float f) {
//    { a += b } -> same_as<T&>;
//    { a -= b } -> same_as<T&>;
//    { a *= f } -> same_as<T&>;
//};

export template <typename ANIM_TYPE>
class Animation
{
public:

    //static_assert(IsLinearCombinable<ANIM_TYPE>, "ANIM_TYPE must be linear combinable");

    Animation(auto const &func, DWORD const dwFlags = 0)
      : m_func(func),
        m_dwFlags(dwFlags)
    {}

    void Start     // runs in UI thread
    (
        ANIM_TYPE      * pAnimated, 
        ANIM_TYPE const& start,
        ANIM_TYPE const& target
    )
    {
        m_pAnimated      = pAnimated;
        m_start          = start;
        m_distance       = target - m_start;
        m_bTargetReached = false;
        setActual(m_start);
        m_smoothMove.Start(m_uiNrOfSteps);
        m_timer.StartTimer
        (
            m_uiMsPeriod, 
            [](PTP_CALLBACK_INSTANCE, PVOID pContext, PTP_TIMER)
            {
                Animation<ANIM_TYPE>* pAnim { bit_cast<Animation<ANIM_TYPE>*>(pContext) };
                if (! pAnim->m_bTargetReached)
                    pAnim->next();
            },
            this
        );
    }

    void Update()
    {
        AcquireSRWLockExclusive(& m_srwlData);
        *m_pAnimated = m_actual;
        ReleaseSRWLockExclusive(& m_srwlData);
    }

    void SetNrOfSteps(unsigned int const uiNrOfSteps)
    {
        m_uiNrOfSteps = uiNrOfSteps;
    }

    void SetMsPeriod(unsigned int const uiMsPeriod)
    {
        m_uiMsPeriod = uiMsPeriod;
    }

    bool TargetReached() { return m_bTargetReached; }

private:

    function<void(bool const)> m_func; 

    ANIM_TYPE * m_pAnimated {};
    ANIM_TYPE   m_actual    {};
    ANIM_TYPE   m_start     {};
    ANIM_TYPE   m_distance  {};

    SmoothMoveFp m_smoothMove;
    DWORD  const m_dwFlags        { 0 };
    SRWLOCK      m_srwlData       { 0 };
    unsigned int m_uiMsPeriod     { 50 };
    unsigned int m_uiNrOfSteps    { 20 };
    bool         m_bTargetReached { false };

    ThreadPoolTimer m_timer;

    void setActual(ANIM_TYPE const newVal)
    {
        AcquireSRWLockExclusive(& m_srwlData);
        m_actual = newVal;
        ReleaseSRWLockExclusive(& m_srwlData);
    }

    void next() // runs in animation thread
    {
        m_bTargetReached = m_smoothMove.Next();
        setActual(m_start + m_distance * m_smoothMove.GetPos());
        if (m_bTargetReached)
        {
            if (m_dwFlags & ANIMATION_RECURRING)
                setActual(m_start);
            else
                m_timer.StopTimer();
        }
        m_func(m_bTargetReached);
    }
};
