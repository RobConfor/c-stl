#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

// A priority queue implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class priority_queue {
public:
    class Node {
    public:
        explicit Node(const TYPE &val) : elt(val) {}

        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        friend class priority_queue;

    private:
        TYPE elt;
        Node *child = nullptr;
        Node *sibling = nullptr;
        Node *parent = nullptr;
    };

    explicit priority_queue(COMP_FUNCTOR comp = COMP_FUNCTOR())
        : compare(std::move(comp)), num_nodes(0), root(nullptr) {}

    template<typename InputIterator>
    priority_queue(InputIterator start, InputIterator end,
                   COMP_FUNCTOR comp = COMP_FUNCTOR())
        : compare(std::move(comp)), num_nodes(0), root(nullptr) {
        while (start != end) {
            push(*start);
            ++start;
        }
    }

    priority_queue(const priority_queue &other)
        : compare(other.compare), num_nodes(0), root(nullptr) {
        if (other.root == nullptr) {
            return;
        }

        std::vector<Node *> pending{other.root};
        while (!pending.empty()) {
            Node *node = pending.back();
            pending.pop_back();
            push(node->elt);

            for (Node *child = node->child; child != nullptr; child = child->sibling) {
                pending.push_back(child);
            }
        }
    }

    priority_queue &operator=(const priority_queue &rhs) {
        if (this != &rhs) {
            priority_queue temp(rhs);
            swap(temp);
        }
        return *this;
    }

    priority_queue(priority_queue &&other) noexcept
        : compare(std::move(other.compare)),
          num_nodes(other.num_nodes),
          root(other.root) {
        other.num_nodes = 0;
        other.root = nullptr;
    }

    priority_queue &operator=(priority_queue &&other) noexcept {
        if (this != &other) {
            clear();
            compare = std::move(other.compare);
            num_nodes = other.num_nodes;
            root = other.root;
            other.num_nodes = 0;
            other.root = nullptr;
        }
        return *this;
    }

    ~priority_queue() { clear(); }

    // Rebuild the heap after priorities have been changed in place.
    void updatePriorities() {
        if (root == nullptr) {
            return;
        }

        std::vector<Node *> pending{root};
        std::vector<Node *> nodes;
        while (!pending.empty()) {
            Node *node = pending.back();
            pending.pop_back();
            nodes.push_back(node);

            for (Node *child = node->child; child != nullptr; child = child->sibling) {
                pending.push_back(child);
            }

            node->child = nullptr;
            node->sibling = nullptr;
            node->parent = nullptr;
        }

        root = nullptr;
        for (Node *node : nodes) {
            root = meld(root, node);
        }
    }

    void push(const TYPE &val) { addNode(val); }

    void pop() {
        if (root == nullptr) {
            return;
        }

        Node *old_root = root;
        Node *children = old_root->child;
        delete old_root;
        --num_nodes;

        root = nullptr;
        if (children == nullptr) {
            return;
        }

        std::vector<Node *> subheaps;
        while (children != nullptr) {
            Node *next = children->sibling;
            children->sibling = nullptr;
            children->parent = nullptr;
            subheaps.push_back(children);
            children = next;
        }

        std::vector<Node *> merged;
        for (std::size_t i = 0; i + 1 < subheaps.size(); i += 2) {
            merged.push_back(meld(subheaps[i], subheaps[i + 1]));
        }
        if (subheaps.size() % 2 != 0) {
            merged.push_back(subheaps.back());
        }

        for (auto it = merged.rbegin(); it != merged.rend(); ++it) {
            root = meld(root, *it);
        }
    }

    const TYPE &top() const { return root->elt; }

    [[nodiscard]] std::size_t size() const { return num_nodes; }

    [[nodiscard]] bool empty() const { return num_nodes == 0; }

    // The new value must have at least as high a priority as the old value.
    void updateElt(Node *node, const TYPE &new_value) {
        if (node == nullptr) {
            return;
        }

        node->elt = new_value;
        if (node == root || node->parent == nullptr ||
            !compare(node->parent->elt, node->elt)) {
            return;
        }

        detach(node);
        root = meld(root, node);
    }

    Node *addNode(const TYPE &val) {
        Node *node = new Node(val);
        ++num_nodes;
        root = meld(root, node);
        return node;
    }

private:
    COMP_FUNCTOR compare;
    std::size_t num_nodes;
    Node *root;

    void swap(priority_queue &other) noexcept {
        using std::swap;
        swap(compare, other.compare);
        swap(num_nodes, other.num_nodes);
        swap(root, other.root);
    }

    void clear() noexcept {
        if (root == nullptr) {
            return;
        }

        std::vector<Node *> pending{root};
        while (!pending.empty()) {
            Node *node = pending.back();
            pending.pop_back();
            for (Node *child = node->child; child != nullptr;) {
                Node *next = child->sibling;
                pending.push_back(child);
                child = next;
            }
            delete node;
        }

        root = nullptr;
        num_nodes = 0;
    }

    void detach(Node *node) {
        Node *parent = node->parent;
        if (parent->child == node) {
            parent->child = node->sibling;
        } else {
            Node *previous = parent->child;
            while (previous->sibling != node) {
                previous = previous->sibling;
            }
            previous->sibling = node->sibling;
        }
        node->sibling = nullptr;
        node->parent = nullptr;
    }

    Node *meld(Node *a, Node *b) {
        if (a == nullptr) {
            return b;
        }
        if (b == nullptr) {
            return a;
        }

        if (compare(a->elt, b->elt)) {
            a->sibling = b->child;
            if (a->sibling != nullptr) {
                a->sibling->parent = b;
            }
            b->child = a;
            a->parent = b;
            return b;
        }

        b->sibling = a->child;
        if (b->sibling != nullptr) {
            b->sibling->parent = a;
        }
        a->child = b;
        b->parent = a;
        return a;
    }
};

#endif  // PRIORITY_QUEUE_H
