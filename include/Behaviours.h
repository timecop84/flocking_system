#ifndef BEHAVIOURS_H
#define BEHAVIOURS_H
#include <vector>
#include "boid.h"
#include "ngl_compat/Vector.h"

/*! \brief the behaviour class */
/// @file behaviours.h
/// @brief the behaviour class. Creates the behaviour for our flock.
/// @brief Pseudo code for the behaviours provided by Conrad Parker
/// Availabe from: http://www.kfish.org/boids/pseudocode.html
/// @author Dionysios Toufexis
/// @version 1.0
/// @date 24/6/2012
/// Revision History :5/7/2012
/// @class Behaviours
/// @brief stores the information of the behaviours and sets the final velocity for the flock taking into account the behaviours.


class Behaviours
{
public:

    /// @brief out ctor
    Behaviours();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Calculates the cohesion behaviour of the flock
    /// @param [in] _boidNumber the current boid.
    /// @param [in] _boidList a dynamic array of all the boids.
    void Cohesion(int &_boidNumber, std::vector <Boid*> & _boidList);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Calculates the alignmenth behaviour of the flock
    /// @param [in] _boidNumber the current boid.
    /// @param [in] _boidList a dynamic array of all the boids.
    void Alignment(int & _boidNumber, std::vector <Boid*> & _boidList);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Calculates the seperation behaviour of the flock
    /// @param [in] _boidNumber the current boid.
    /// @param [in] _boidList a dynamic array of all the boids.
    void Seperation(int & _boidNumber, std::vector <Boid*> & _boidList);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our behaviour set method. Sets the final velocity with all the behaviours
    ngl::Vector m_behaviourSet();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Calculates the destination behaviour of the flock
    /// @param [in] _boidNumber the current boid.
    /// @param [in] _boidList a dynamic array of all the boids.
    void Destination(int & _boidNumber, std::vector <Boid*> & _boidList);
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector BehaviourSetup();
    /// @brief GUI related sets for the simulation
    //----------------------------------------------------------------------------------------------------------------------
    void setBehaviourDistance(double distance) {m_BehaviourDistance = distance;}
    void setFlockDistance(double distance) {m_flockDistance = distance;}
    void setCohesionForce(double cohesion) {m_cohesionForce = cohesion;}
    void setSeparationForce(double separation) {m_seperationForce = separation;}
    void setAlignment(double alignment) {m_alignment = alignment;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our ctor
    ~Behaviours();
    //----------------------------------------------------------------------------------------------------------------------

private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our boid pointer
    Boid *m_boid;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our boidlist pointer
    std::vector <Boid*> *_boidList;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the value of cohesion the check.
    ngl::Vector m_coherence;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to check the distance between the behaviours.
    double m_BehaviourDistance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to contribute additional cohesion force to the flock.
    double  m_cohesionForce;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to contribute additional seperation force to the flock.
    double m_seperationForce;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief contribution of the Alignment force in overall behaviour Flock
    double m_alignment;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the amount of Separation generated.
    ngl::Vector m_separation;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to contribute additional alignment force to the flock.
    ngl::Vector m_alignmentForce;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the diffence in positions between the current boid to the local boids.
    ngl::Vector m_boidDistance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the positions between the current boid to the local boids.
    double m_flockDistance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the behaviour movement.
    ngl::Vector m_behaviourSetup;
    /// @brief variable to store the final seperation velocity.
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_seperationSet;
    /// @brief variable to store the final cohesion velocity.
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_cohesionSet;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the final alignment velocity.
    ngl::Vector m_alighmentSet;
    //----------------------------------------------------------------------------------------------------------------------
};

#endif // BEHAVIOURS_H
