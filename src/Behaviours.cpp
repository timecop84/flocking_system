#include "Behaviours.h"

Behaviours::Behaviours()
{
    m_BehaviourDistance = 20;
    m_flockDistance = 4;
    m_seperationForce = 9;
    m_alignment = 10;
    m_cohesionForce = 2;
}
//----------------------------------------------------------------------------------------------------------------------
void Behaviours::Cohesion(int &_boidNumber, std::vector <Boid*> & _boidList)
{
    m_coherence.set(0, 0, 0);
    m_boidDistance.set(0, 0, 0);
    int count = 1;

    for(int i=0;i<_boidList.size();i++)
    {
        if(i!=_boidNumber)
        {
            m_boidDistance = _boidList.at(_boidNumber)->getPosition() - _boidList.at(i)->getPosition();

            if(m_boidDistance.length() < m_BehaviourDistance)
            {
                m_coherence += _boidList.at(i)->getPosition();
                count++;
            }
        }
    }
    m_coherence /= count;
    m_coherence = (m_coherence - _boidList.at(_boidNumber)->getPosition());
    m_coherence.normalize();
}


//----------------------------------------------------------------------------------------------------------------------
void Behaviours::Alignment(int &_boidNumber, std::vector<Boid*> &_boidList)
{
    int count = 1;
    m_boidDistance.set(0, 0, 0);

    for(int i=0;i<_boidList.size();i++)
    {
        if(i!=_boidNumber)
        {
            m_boidDistance = _boidList.at(_boidNumber)->getPosition() - _boidList.at(i)->getPosition();

            if(m_boidDistance.length() < m_BehaviourDistance)
            {
                m_alignmentForce += _boidList.at(i)->getVelocity();
                count++;
            }
        }
    }

    if (m_alignmentForce.length() > m_BehaviourDistance)
    {
        m_alignmentForce.normalize();
    }

    m_alignmentForce /= count;
    m_alignmentForce = (m_alignmentForce - _boidList.at(_boidNumber)->getVelocity());


}
//----------------------------------------------------------------------------------------------------------------------
void Behaviours::Seperation(int &_boidNumber, std::vector<Boid*> &_boidList)
{
    m_separation.set(0, 0, 0);
    m_boidDistance.set(0, 0, 0);

    for(int i=0;i<_boidList.size();i++)
    {
        if(i!=_boidNumber)
        {
            m_boidDistance = _boidList.at(_boidNumber)->getPosition() - _boidList.at(i)->getPosition();

            if(m_boidDistance.length() < m_flockDistance)
            {
                m_separation -= (_boidList.at(_boidNumber)->getPosition() - _boidList.at(i)->getPosition());
            }
        }
    }
    if (m_boidDistance.length() > m_flockDistance)
    {
        m_boidDistance.normalize();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Behaviours::Destination(int & _boidNumber, std::vector <Boid*> & _boidList)
{
    ngl::Vector targeting(1,0,1);

    targeting = ((targeting   - _boidList.at(_boidNumber)->getNextPosition()) * - 100);
}
//----------------------------------------------------------------------------------------------------------------------

ngl::Vector Behaviours::BehaviourSetup()
{

    ngl::Vector m_seperationCorrection(-1, -1, -1);
    m_behaviourSetup.set(0, 0, 0);


    m_seperationSet = (m_seperationForce * m_separation) * m_seperationCorrection;
    m_cohesionSet = m_cohesionForce * m_coherence;
    m_alighmentSet = m_alignmentForce * m_alignment;
    m_behaviourSetup = m_seperationSet + m_cohesionSet + m_alighmentSet;

    if (m_behaviourSetup.length() > 0.5)
    {
        m_behaviourSetup.normalize();
        m_behaviourSetup *= 0.5;
    }


    return m_behaviourSetup;





}

