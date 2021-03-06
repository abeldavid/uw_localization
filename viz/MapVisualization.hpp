#ifndef UW_PARTICLE_LOCALIZATION_MAPVIZ_HPP
#define UW_PARTICLE_LOCALIZATION_MAPVIZ_HPP

#include <boost/noncopyable.hpp>
#include <vizkit3d/Vizkit3DPlugin.hpp>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <uw_localization/types/environment.hpp>
#include <base/samples/RigidBodyState.hpp>
#include <QString>

namespace vizkit3d {

class MapVisualization
    : public vizkit3d::Vizkit3DPlugin<uw_localization::Environment>,
    boost::noncopyable
{
    Q_OBJECT
    Q_PROPERTY(QString map WRITE setMap)
    Q_PROPERTY(int grid_resolution WRITE setGridResolution)

 public:
     MapVisualization() {
        VizPluginRubyAdapter(MapVisualization, uw_localization::Environment,  Map);
        updated = false;
        resolution = 1.0;
     }

     ~MapVisualization();

     void setMap(const QString& p);
     void setGridResolution(const int& resolution);
     
      Q_INVOKABLE void updateEnv( const uw_localization::Environment& sample )
        {
	  return updateData(sample); }     
     

 protected:
     virtual osg::ref_ptr<osg::Node> createMainNode();
     virtual void updateMainNode(osg::Node* node);
     virtual void updateDataIntern(const uw_localization::Environment& env);

     void renderEnvironment(const uw_localization::Environment& env);
     void updatePlaneNode(osg::Geode* geode, const uw_localization::Plane& plane);

 private:
     osg::ref_ptr<osg::Vec3Array> border_points;
     osg::ref_ptr<osg::Vec4Array> border_colors;
     osg::ref_ptr<osg::Geometry>  border_geom;
     osg::ref_ptr<osg::DrawArrays> grid;

     osg::ref_ptr<osg::Group> landmark_group;
     osg::ref_ptr<osg::Vec3Array> landmark_points;
     osg::ref_ptr<osg::Vec4Array> landmark_colors;
     osg::ref_ptr<osg::Geometry>  landmark_geom;
     
     osg::ref_ptr<osg::Group> plane_group;
     osg::ref_ptr<osg::Vec3Array> plane_points;
     osg::ref_ptr<osg::Vec4Array> plane_colors;
     osg::ref_ptr<osg::Geometry>  plane_geom;
     osg::ref_ptr<osg::DrawArrays> plane_draw_array;
     

     bool updated;
     int resolution;

     uw_localization::Environment data_env;
};

}

#endif // UW_PARTICLE_LOCALIZATION_STATICMAPVIZ_HPP


