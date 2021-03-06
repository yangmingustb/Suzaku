// Copyright 2020 The Autoware Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/// \copyright Copyright 2020 The Autoware Foundation
/// \file
/// \brief This file defines the lanelet2_map_provider_node class.

#ifndef LANELET2_MAP_PROVIDER__LANELET2_MAP_PROVIDER_NODE_HPP_
#define LANELET2_MAP_PROVIDER__LANELET2_MAP_PROVIDER_NODE_HPP_

#include <rclcpp/rclcpp.hpp>

#include <lanelet2_map_provider/lanelet2_map_provider.hpp>
#include <common/types.hpp>
#include <string>
#include <memory>

#include "autoware_auto_msgs/srv/had_map_service.hpp"
#include "autoware_auto_msgs/msg/had_map_bin.hpp"


using autoware::common::types::float64_t;
using autoware::common::types::bool8_t;
namespace autoware
{
namespace lanelet2_map_provider
{

/// \class Lanelet2MapProviderNode
/// \brief ROS 2 Node for semantic map provider using lanelet2 map format.
/// Node loads a lanelet2 OSM format map using the class Lanelet2MapProvider
/// and then runs a service for supplying other
/// nodes map information according to their requests. Requests are defined by
/// a sequences of requested primitives as well as geometric bounds on requested map
/// data

class LANELET2_MAP_PROVIDER_PUBLIC Lanelet2MapProviderNode : public rclcpp::Node
{
public:
  /// \brief default constructor, starts driver
  /// *breaks docs test - no node_nmae generated by automatic package generator
  /// -- param[in] node_name name of the node for rclcpp internals
  /// \throw runtime error if failed to start threads or configure driver
  explicit Lanelet2MapProviderNode(const rclcpp::NodeOptions & options);

  /// \brief load_map loads the osm map data while projecting into the coordinate
  /// system defined by the map origin
  void load_map();

  /// \brief Initialisation of the node Create the map provider object from the given map filename,
  /// and set up the service call handler for the node
  void init();

  /// \brief Handles the node service requests
  /// \param request Service request message for map data specifying map content and geom. bounds
  /// \param response Service repsone to request, containing a sub-set of map data
  /// but nethertheless containing a complete and valid lanelet2 map
  void handle_request(
    std::shared_ptr<autoware_auto_msgs::srv::HADMapService_Request> request,
    std::shared_ptr<autoware_auto_msgs::srv::HADMapService_Response> response);

private:
  /// If the origin not defined by parameters, get the transform describing
  /// the map origin (earth->map transform, set by the pcd
  /// map provider). Geocentric  lanelet2 coordinates can then be projected intro the map frame.
  /// Must be recieved before the node can call the map provider constructor, and start the
  /// service handler
  void get_map_origin();
  float64_t m_origin_lat;  /// map orgin in latitude, longitude and elevatopm
  float64_t m_origin_lon;
  float64_t m_origin_ele;
  bool8_t m_origin_set;
  bool8_t m_verbose;  ///< whether to use verbose output or not.
  std::string m_map_filename;
  std::unique_ptr<Lanelet2MapProvider> m_map_provider;
  rclcpp::Service<autoware_auto_msgs::srv::HADMapService>::SharedPtr m_map_service;
  geometry_msgs::msg::TransformStamped m_earth_to_map;   /// map origin in ECEF ENU transform
};

}  // namespace lanelet2_map_provider

}  // namespace autoware

#endif  // LANELET2_MAP_PROVIDER__LANELET2_MAP_PROVIDER_NODE_HPP_
