#include "algo_rrt.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include <limits>
#include <rclcpp/rclcpp.hpp>

RRT_Planner::RRT_Planner() {
  RCLCPP_INFO(rclcpp::get_logger("RRT_Planner"), "RRT Planner initialized");
}

void RRT_Planner::setDomain(std::vector<int>& domain) {
  std::fill(DOMAIN.begin(), DOMAIN.end(), 0);
  std::copy(domain.begin(), domain.end(), DOMAIN.begin());
}

NodeRrt RRT_Planner::generateRandomNode(std::mt19937& gen) {
  std::uniform_int_distribution<int> distribution(0, GRID_WIDTH * GRID_HEIGHT - 1);
  int random_index = distribution(gen);
  RCLCPP_INFO(rclcpp::get_logger("RRT_Planner"), "Generated random node at index %d", random_index);
  return indexToCoordinate(random_index);
}

NodeRrt RRT_Planner::findNearestNode(std::vector<NodeRrt> const& nodes, NodeRrt const& randomNode) {
  NodeRrt nearestNode = nodes[0];
  double minDistance = std::numeric_limits<double>::max();

  for (auto const& node : nodes) {
    double distance = NodeRrt::heuristics(node, randomNode);
    if (distance < minDistance) {
      minDistance = distance;
      nearestNode = node;
    }
  }

  RCLCPP_INFO(rclcpp::get_logger("RRT_Planner"), "Found nearest node at (%d, %d)", nearestNode.getX(), nearestNode.getY());
  return nearestNode;
}

NodeRrt RRT_Planner::findNewConfig(NodeRrt const& nearestNode, NodeRrt const& randomNode) {
  int dx = randomNode.getX() - nearestNode.getX();
  int dy = randomNode.getY() - nearestNode.getY();

  float distance = NodeRrt::heuristicsEuclid(nearestNode, randomNode);

  float scaledDx = (distance > 0) ? (dx * STEP_SIZE) / distance : dx;
  float scaledDy = (distance > 0) ? (dy * STEP_SIZE) / distance : dy;

  int x = nearestNode.getX() + static_cast<int>(scaledDx);
  int y = nearestNode.getY() + static_cast<int>(scaledDy);

  RCLCPP_INFO(rclcpp::get_logger("RRT_Planner"), "Found new configuration at (%d, %d)", x, y);
  return NodeRrt(x, y);
}

bool RRT_Planner::isObstacle(NodeRrt const& nearestNode, NodeRrt const& newNode) {
  if (newNode.getX() < 0 || newNode.getX() >= GRID_WIDTH || newNode.getY() < 0 || newNode.getY() >= GRID_HEIGHT) {
    RCLCPP_WARN(rclcpp::get_logger("RRT_Planner"), "New node (%d, %d) is out of bounds", newNode.getX(), newNode.getY());
    return true;
  }

  if (DOMAIN[newNode.getY() * GRID_WIDTH + newNode.getX()] == 100) {
    RCLCPP_WARN(rclcpp::get_logger("RRT_Planner"), "New node (%d, %d) is an obstacle", newNode.getX(), newNode.getY());
    return true;
  }

  int dx = newNode.getX() - nearestNode.getX();
  int dy = newNode.getY() - nearestNode.getY();
  int steps = std::max(std::abs(dx), std::abs(dy));

  for (int i = 0; i < steps; i++) {
    float x = nearestNode.getX() + (i * dx) / steps;
    float y = nearestNode.getY() + (i * dy) / steps;

    if (DOMAIN[static_cast<int>(y) * GRID_WIDTH + static_cast<int>(x)] == 100) {
      RCLCPP_WARN(rclcpp::get_logger("RRT_Planner"), "Path from (%d, %d) to (%d, %d) is blocked by an obstacle", nearestNode.getX(), nearestNode.getY(), newNode.getX(), newNode.getY());
      return true;
    }
  }

  return false;
}

bool RRT_Planner::isGoalFound(NodeRrt& newNode, NodeRrt const& goal) {
  return newNode == goal;
}

NodeRrt RRT_Planner::indexToCoordinate(int index) {
  int x = index % GRID_WIDTH;
  int y = index / GRID_WIDTH;

  return NodeRrt(x, y);
}

std::vector<NodeRrt> RRT_Planner::findNearbyNodes(NodeRrt const& new_node, float radius) {
  std::vector<NodeRrt> nearby_nodes;
  for (const auto& node : nodes) {
    if (NodeRrt::heuristicsEuclid(node, new_node) <= radius) {
      nearby_nodes.push_back(node);
    }
  }
  return nearby_nodes;
}

void RRT_Planner::rewire(NodeRrt& new_node, std::vector<NodeRrt>& nearby_nodes) {
  for (auto& node : nearby_nodes) {
    if (!isObstacle(new_node, node)) {
      float new_cost = new_node.getCost() + NodeRrt::heuristicsEuclid(new_node, node);
      if (new_cost < node.getCost()) {
        node.setParent(std::make_shared<NodeRrt>(new_node));
        node.setCost(new_cost);
      }
    }
  }
}

std::vector<NodeRrt> RRT_Planner::planPath(NodeRrt const& start, NodeRrt const& goal) {
  nodes.clear();
  path.clear();
  nodes.push_back(start);
  std::mt19937 gen(SEED);

  for (int i = 0; i < MAX_ITERATIONS; i++) {
    NodeRrt randomNode = generateRandomNode(gen);
    NodeRrt nearestNode = findNearestNode(nodes, randomNode);
    if (nearestNode == randomNode) {
      continue;
    }

    NodeRrt newNode = findNewConfig(nearestNode, randomNode);

    if (isObstacle(nearestNode, newNode)) {
      continue;
    }

    newNode.setParent(std::make_shared<NodeRrt>(nearestNode));
    newNode.setCost(nearestNode.getCost() + NodeRrt::heuristicsEuclid(nearestNode, newNode));
    nodes.push_back(newNode);

    std::vector<NodeRrt> nearby_nodes = findNearbyNodes(newNode, REWIRE_RADIUS);
    rewire(newNode, nearby_nodes);

    if (isGoalFound(newNode, goal)) {
      path.push_back(newNode);
      auto parent = *newNode.getParent();

      while (&parent != &start) {
        path.push_back(parent);
        if (parent.getParent() == nullptr) {
          break;
        }

        parent = *parent.getParent();
      }
      std::reverse(path.begin(), path.end());
      break;
    }
  }

  return path;
}