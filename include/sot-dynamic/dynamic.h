/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 * This file is part of sot-dynamic.
 * sot-dynamic is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * sot-dynamic is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with sot-dynamic.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SOT_DYNAMIC_H__
#define __SOT_DYNAMIC_H__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* STD */
#include <string>
#include <map>

/* Matrix */
#include <dynamic-graph/linear-algebra.h>
#include <jrl/mal/matrixabstractlayer.hh>

/* JRL dynamic */
#include <abstract-robot-dynamics/humanoid-dynamic-robot.hh>
#include <jrl/dynamics/dynamicsfactory.hh>

namespace djj = dynamicsJRLJapan;

/* SOT */
#include <sot/core/flags.hh>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <sot/core/exception-dynamic.hh>
#include <sot/core/matrix-geometry.hh>

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32)
#  if defined (dynamic_EXPORTS)
#    define SOTDYNAMIC_EXPORT __declspec(dllexport)
#  else
#    define SOTDYNAMIC_EXPORT __declspec(dllimport)
#  endif
#else
#  define SOTDYNAMIC_EXPORT
#endif


namespace dynamicgraph { namespace sot {
namespace dg = dynamicgraph;

  namespace command {
    class SetFiles;
    class Parse;
    class CreateOpPoint;
    class InitializeRobot;
  }
/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/*! @ingroup signals
  \brief This class provides an inverse dynamic model of the robot.
  More precisely it wraps the newton euler algorithm implemented
  by the dynamicsJRLJapan library to make it accessible in the stack of tasks.
  The robot is described by a VRML file.
*/

class SOTDYNAMIC_EXPORT Dynamic
:public dg::Entity
{
  friend class sot::command::SetFiles;
  friend class sot::command::Parse;
  friend class sot::command::CreateOpPoint;
  friend class sot::command::InitializeRobot;

 public:
  DYNAMIC_GRAPH_ENTITY_DECL();

 protected:
 public:

  /*! \name Fields to access dynamicsJRLJapan Library
    @{
   */

  /*! \brief Abstract pointer on the structure.
    Ultimately we should be able to use only the abstract
    interface and do not care about the implementation.
  */
  CjrlHumanoidDynamicRobot* m_HDR;


  int debugInertia;

  /*! \brief Fields to access the humanoid model
   @{ */

  /*! \brief Directory where the VRML humanoid model is stored */
  std::string vrmlDirectory;
  /*! \brief Name of the root's robot model file */
  std::string vrmlMainFile;
  /*! \brief Name of the name specifying which end-effector is
    the head, the feet and so on... */
  std::string xmlSpecificityFile;
  /*! \brief Name of the name specifying which end-effector is
    the head, the feet and so on... */
  std::string xmlRankFile;
  /*! @} */

  /*! @} */
  bool init;
  std::list< dg::SignalBase<int>*  > genericSignalRefs;

 public: /* --- CONSTRUCTION --- */

  Dynamic( const std::string& name, bool build=true );
  virtual ~Dynamic( void );

 public: /* --- MODEL CREATION --- */

  virtual void buildModel( void );

  void setVrmlDirectory( const std::string& filename );
  void setVrmlMainFile( const std::string& filename );
  void setXmlSpecificityFile( const std::string& filename );
  void setXmlRankFile( const std::string& filename );
  void parseConfigFiles( void );

 public: /* --- SIGNAL ACTIVATION --- */
  dg::SignalTimeDependent< dynamicgraph::Matrix,int > &
    createEndeffJacobianSignal( const std::string& signame,
				CjrlJoint* inJoint );
  dg::SignalTimeDependent< dynamicgraph::Matrix,int > &
    createJacobianSignal( const std::string& signame,
			  CjrlJoint* inJoint );
  void destroyJacobianSignal( const std::string& signame );
  dg::SignalTimeDependent< MatrixHomogeneous,int >&
    createPositionSignal( const std::string& signame,
			  CjrlJoint* inJoint );
  void destroyPositionSignal( const std::string& signame );
  dg::SignalTimeDependent< dynamicgraph::Vector,int >&
      createVelocitySignal( const std::string& signame,
  			     CjrlJoint* inJoint );
    void destroyVelocitySignal( const std::string& signame );
  dg::SignalTimeDependent< dynamicgraph::Vector,int >&
    createAccelerationSignal( const std::string& signame,
			     CjrlJoint* inJoint );
  void destroyAccelerationSignal( const std::string& signame );

  bool zmpActivation( void ) { std::string Property("ComputeZMP");
    std::string Value; m_HDR->getProperty(Property,Value); return (Value=="true");}
  void zmpActivation( const bool& b ) { std::string Property("ComputeZMP");
    std::string Value; if (b) Value="true"; else Value="false"; m_HDR->setProperty(Property,Value); }
  bool comActivation( void ) { std::string Property("ComputeCoM");
    std::string Value; m_HDR->getProperty(Property,Value); return (Value=="true"); }
  void comActivation( const bool& b ) { std::string Property("ComputeCoM");
    std::string Value; if (b) Value="true"; else Value="false"; m_HDR->setProperty(Property,Value); }

 public: /* --- SIGNAL --- */

  dg::SignalPtr<dynamicgraph::Vector,int> jointPositionSIN;
  dg::SignalPtr<dynamicgraph::Vector,int> freeFlyerPositionSIN;
  dg::SignalPtr<dynamicgraph::Vector,int> jointVelocitySIN;
  dg::SignalPtr<dynamicgraph::Vector,int> freeFlyerVelocitySIN;
  dg::SignalPtr<dynamicgraph::Vector,int> jointAccelerationSIN;
  dg::SignalPtr<dynamicgraph::Vector,int> freeFlyerAccelerationSIN;

  // protected:
 public:
  typedef int Dummy;
  dg::SignalTimeDependent<Dummy,int> firstSINTERN;
  dg::SignalTimeDependent<Dummy,int> newtonEulerSINTERN;

  int& computeNewtonEuler( int& dummy,int time );
  int& initNewtonEuler( int& dummy,int time );

 public:
  dg::SignalTimeDependent<dynamicgraph::Vector,int> zmpSOUT;
  dg::SignalTimeDependent<dynamicgraph::Matrix,int> JcomSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> comSOUT;
  dg::SignalTimeDependent<dynamicgraph::Matrix,int> inertiaSOUT;

  dg::SignalTimeDependent<dynamicgraph::Matrix,int>& jacobiansSOUT( const std::string& name );
  dg::SignalTimeDependent<MatrixHomogeneous,int>& positionsSOUT( const std::string& name );
  dg::SignalTimeDependent<dynamicgraph::Vector,int>& velocitiesSOUT( const std::string& name );
  dg::SignalTimeDependent<dynamicgraph::Vector,int>& accelerationsSOUT( const std::string& name );

  dg::SignalTimeDependent<double,int> footHeightSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> upperJlSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> lowerJlSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> upperVlSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> lowerVlSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> upperTlSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> lowerTlSOUT;

  dg::Signal<dynamicgraph::Vector,int> inertiaRotorSOUT;
  dg::Signal<dynamicgraph::Vector,int> gearRatioSOUT;
  dg::SignalTimeDependent<dynamicgraph::Matrix,int> inertiaRealSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> MomentaSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> AngularMomentumSOUT;
  dg::SignalTimeDependent<dynamicgraph::Vector,int> dynamicDriftSOUT;

 protected:
  dynamicgraph::Vector& computeZmp( dynamicgraph::Vector& res,int time );
  dynamicgraph::Vector& computeMomenta( dynamicgraph::Vector &res, int time);
  dynamicgraph::Vector& computeAngularMomentum( dynamicgraph::Vector &res, int time);
  dynamicgraph::Matrix& computeJcom( dynamicgraph::Matrix& res,int time );
  dynamicgraph::Vector& computeCom( dynamicgraph::Vector& res,int time );
  dynamicgraph::Matrix& computeInertia( dynamicgraph::Matrix& res,int time );
  dynamicgraph::Matrix& computeInertiaReal( dynamicgraph::Matrix& res,int time );
  double& computeFootHeight( double& res,int time );

  dynamicgraph::Matrix& computeGenericJacobian( CjrlJoint* j,dynamicgraph::Matrix& res,int time );
  dynamicgraph::Matrix& computeGenericEndeffJacobian( CjrlJoint* j,dynamicgraph::Matrix& res,int time );
  MatrixHomogeneous& computeGenericPosition( CjrlJoint* j,MatrixHomogeneous& res,int time );
  dynamicgraph::Vector& computeGenericVelocity( CjrlJoint* j,dynamicgraph::Vector& res,int time );
  dynamicgraph::Vector& computeGenericAcceleration( CjrlJoint* j,dynamicgraph::Vector& res,int time );

  dynamicgraph::Vector& getUpperJointLimits( dynamicgraph::Vector& res,const int& time );
  dynamicgraph::Vector& getLowerJointLimits( dynamicgraph::Vector& res,const int& time );

  dynamicgraph::Vector& getUpperVelocityLimits( dynamicgraph::Vector& res,const int& time );
  dynamicgraph::Vector& getLowerVelocityLimits( dynamicgraph::Vector& res,const int& time );

  dynamicgraph::Vector& getUpperTorqueLimits( dynamicgraph::Vector& res,const int& time );
  dynamicgraph::Vector& getLowerTorqueLimits( dynamicgraph::Vector& res,const int& time );

  dynamicgraph::Vector& computeTorqueDrift( dynamicgraph::Vector& res,const int& time );

 public: /* --- PARAMS --- */
  virtual void commandLine( const std::string& cmdLine,
			    std::istringstream& cmdArgs,
			    std::ostream& os );
  void cmd_createOpPointSignals           ( const std::string& sig,const std::string& j );
  void cmd_createJacobianWorldSignal      ( const std::string& sig,const std::string& j );
  void cmd_createJacobianEndEffectorSignal( const std::string& sig,const std::string& j );
  void cmd_createPositionSignal           ( const std::string& sig,const std::string& j );

 public:
  /// \name Construction of a robot by commands
  ///@{
  ///

  /// \brief Create an empty device
  void createRobot();

  /// \brief create a joint
  /// \param inJointName name of the joint,
  /// \param inJointType type of joint in ["freeflyer","rotation","translation","anchor"],
  /// \param inPosition position of the joint (4x4 homogeneous matrix).
  ///
  /// \note joints are stored in a map with names as keys for retrieval by other
  /// commands. An empty CjrlBody is also created and attached to the joint.
  void createJoint(const std::string& inJointName,
		   const std::string& inJointType,
		   const dynamicgraph::Matrix& inPosition);

  /// \brief Set a joint as root joint of the robot.
  void setRootJoint(const std::string& inJointName);

  /// \brief Add a child joint to a joint.
  /// \param inParentName name of the joint to which a child is added.
  /// \param inChildName name of the child joint added.
  void addJoint(const std::string& inParentName,
		const std::string& inChildName);

  /// \brief Set bound of degree of freedom
  ///
  /// \param inJointName name of the joint,
  /// \param inDofId id of the degree of freedom in the joint,
  /// \param inMinValue, inMaxValue values of degree of freedom bounds

  void setDofBounds(const std::string& inJointName, unsigned int inDofId,
		    double inMinValue, double inMaxValue);

  /// \brief Set mass of a body
  ///
  /// \param inJointName name of the joint to which the body is attached,
  /// \param inMass mass of the body
  void setMass(const std::string& inJointName, double inMass);

  /// \brief Set local center of mass of a body
  ///
  /// \param inJointName name of the joint to which the body is attached,
  /// \param inCom local center of mass.
  void setLocalCenterOfMass(const std::string& inJointName, dynamicgraph::Vector inCom);

  /// \brief Set inertia matrix of a body
  ///
  /// \param inJointName name of the joint to which the body is attached,
  /// \param inMatrix inertia matrix.
  void setInertiaMatrix(const std::string& inJointName,
			dynamicgraph::Matrix inMatrix);

  /// \brief Set specific joints
  ///
  /// \param inJointName name of the joint,
  /// \param inJointType type of joint in ['chest','waist','left-wrist','right-wrist','left-ankle','right-ankle','gaze'].
  void setSpecificJoint(const std::string& inJointName,
			const std::string& inJointType);

  /// \brief Set hand parameters
  ///
  /// \param inRight true if right hand, false if left hand,
  /// \param inCenter center of the hand in wrist local frame,
  /// \param inThumbAxis thumb axis in wrist local frame,
  /// \param inForefingerAxis forefinger axis in wrist local frame,
  /// \param inPalmNormalAxis palm normal in wrist local frame,
  void setHandParameters(bool inRight, const dynamicgraph::Vector& inCenter,
			 const dynamicgraph::Vector& inThumbAxis,
			 const dynamicgraph::Vector& inForefingerAxis,
			 const dynamicgraph::Vector& inPalmNormal);

  /// \brief Set foot parameters
  ///
  /// \param inRight true if right foot, false if left foot,
  /// \param inSoleLength sole length,
  /// \param inSoleWidth sole width,
  /// \param inAnklePosition ankle position in foot local frame,
  void setFootParameters(bool inRight, const double& inSoleLength,
			 const double& inSoleWidth,
			 const dynamicgraph::Vector& inAnklePosition);

  /// \brief Set gaze parameters
  ///
  /// \param inGazeOrigin origin of the gaze in gaze joint local frame,
  /// \param inGazeDirection direction of the gase in gaze joint local frame.
  void setGazeParameters(const dynamicgraph::Vector& inGazeOrigin,
			 const dynamicgraph::Vector& inGazeDirection);

  /// \brief Get length of left foot sole.
  ///
  /// The robot is assumed to be symmetric.
  double getSoleLength() const;

  /// \brief Get width of left foot sole.
  ///
  /// The robot is assumed to be symmetric.
  double getSoleWidth() const;

  /// \brief Get left ankle position in foot frame
  ///
  /// The robot is assumed to be symmetric.
  dynamicgraph::Vector getAnklePositionInFootFrame() const;

  /// @}
  ///
 private:
  /// \brief map of joints in construction.
  std::map<std::string, CjrlJoint*> jointMap_;
  djj::ObjectFactory factory_;
  /// Return a specific joint, being given a name by string inside a short list.
  CjrlJoint* getJointByName( const std::string& jointName );

};

  std::ostream& operator<<(std::ostream& os, const CjrlHumanoidDynamicRobot& r);
  std::ostream& operator<<(std::ostream& os, const CjrlJoint& r);
} /* namespace sot */} /* namespace dynamicgraph */



#endif // #ifndef __SOT_DYNAMIC_H__
