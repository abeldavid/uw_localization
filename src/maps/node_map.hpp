#ifndef UW_LOCALIZATION_MAPS_STOCHASTIC_MAPS
#define UW_LOCALIZATION_MAPS_STOCHASTIC_MAPS

#include <string>
#include <yaml-cpp/yaml.h>
#include <Eigen/Core>
#include <machine_learning/GaussianParameters.hpp>
#include <machine_learning/RandomNumbers.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include "map.hpp"
#include "../types/map.hpp"

namespace uw_localization {

const int NODE_GROUP = 0;
const int NODE_LANDMARK = 1;

class LandmarkNode;

class Node {
public:
  Node(const std::string& caption = "");
  virtual ~Node();
  
  void addChild(Node* child);
  Node* getChild(unsigned index);
  void removeChild(unsigned index);
  void removeChild(Node* child);

  unsigned getChildSize() const;

  virtual int getNodeType() const { return NODE_GROUP; }
  
  virtual boost::tuple<LandmarkNode*, double> getProbability(const std::string& caption, const Eigen::Vector3d& v);

  std::vector<LandmarkNode*> getLandmarks(const std::string& caption = "");

  const std::string& getCaption() const { return caption; }
  std::string& getCaption() { return caption; }

private:
  std::vector<Node*> children;
  std::string caption;
};


class LandmarkNode : public Node {
public:
  LandmarkNode(const Eigen::Vector3d& mean, const Eigen::Matrix3d& cov, const std::string& caption = "");
  virtual ~LandmarkNode();

  virtual int getNodeType() const { return NODE_LANDMARK; }
  
  virtual boost::tuple<LandmarkNode*, double> getProbability(const std::string& caption, const Eigen::Vector3d& v);

  Eigen::Vector3d draw();

  const Eigen::Vector3d& mean() const { return params.mean; }
  const Eigen::Matrix3d& covariance() const { return params.covariance; }

private:
  machine_learning::GaussParam<3> params;
  machine_learning::MultiNormalRandom<3> drawer;
};


class NodeMap : public Map {
public:
  NodeMap(const std::string& map);
  NodeMap(const Eigen::Vector3d& limits, const Eigen::Translation3d& t, Node* root);
  virtual ~NodeMap();

  virtual std::vector<boost::tuple<LandmarkNode*, Eigen::Vector3d> > drawSamples(const std::string& caption, int numbers);
  virtual boost::tuple<LandmarkNode*, double> getProbability(const std::string& caption, const Eigen::Vector3d& v);
  virtual MixedMap getMap();

  bool toYaml(std::ostream& stream);
  bool fromYaml(std::istream& stream);

private: 
  void parseYamlNode(const YAML::Node& node, Node* root);

private:
  Node* root;
};

void operator>>(const YAML::Node& node, Eigen::Vector3d& v);
void operator>>(const YAML::Node& node, Eigen::Matrix3d& cov);
}

#endif