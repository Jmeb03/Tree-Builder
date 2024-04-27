#include <iostream>
#include <string>
#include <vector>
#include <memory> // For smart pointers

// Forward declaration to allow parent-child relationships
class TreeNode;

using TreeNodePtr = std::shared_ptr<TreeNode>; // Define a smart pointer to TreeNode

// TreeNode class definition
class TreeNode {
public:
    TreeNode() : name(""), weight(0), parent(nullptr), children() {}
    // Constructor
    TreeNode(const std::string& name, double weight, TreeNode* parent = nullptr)
        : name(name), weight(weight), parent(parent), children() {}

    // Add a child to the node
    void addChild(TreeNode& child) {
        children.push_back(child);
    }

    // Get the name of the node
    std::string getName() const {
        return name;
    }

    // Get the weight of the node
    double getWeight() const {
        return weight;
    }

    // Get the parent of the node
    TreeNode* getParent() const {
        return parent;
    }

    void setParent(TreeNode& parent) {
        this->parent = &parent;
    }

    // Get the children of the node
    std::vector<TreeNode> getChildren() const {
        return children;
    }

    // Display the node and its children recursively
    void display(int depth = 0) const {
        std::string indent(depth * 2, ' '); // Indentation to represent tree levels
        std::cout << indent << "(<" << name << "," << weight << ">"; // Display current node
        
        if (!children.empty()) {
            std::cout << ","; // If there are children, add a comma
            for (size_t i = 0; i < children.size(); ++i) {

                std::cout << "Displaying child: " << children[i].getName() << std::endl; // Debug info

                children[i].display(depth + 1); // Recursive call to display children
                if (i < children.size() - 1) {
                    std::cout << ","; // Comma between children
                }
            }
        }
        
        std::cout << ")"; // Close the representation
    }

private:
    std::string name; // Name of the node
    int weight;    // Weight of the node
    TreeNode* parent; // Parent node (nullptr if root)
    std::vector<TreeNode> children; // List of child nodes
};
