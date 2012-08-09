/*
 * Copyright (C) 2011 Mathieu Turcotte (mathieuturcotte.ca)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef EXACT_COVER_SOLVER_HPP_
#define EXACT_COVER_SOLVER_HPP_

#include <vector>
#include <limits>

#include "types.hpp"

namespace exact_cover {

namespace details {

// A node contains pointer to all of its neighbors (left,
// right, up, down), a pointer to the header node of its
// column and a field which stores either the number of
// elements in a column or the row associated to a given
// node in the original binary matrix used to express the
// exact cover instance.
struct Node {
    Node* header;
    Node* up, *down;
    Node* left, *right;
    uint32 data;
};

// Allocate and initialize the cover matrix root.
Node* build_root() {
    Node* root = new Node;
    root->down = root;
    root->up = root;
    root->left = root;
    root->right = root;
    return root;
}

// Allocate and initialize cover matrix column headers.
std::vector<Node*> build_column_headers(Node* root, size_t cols) {
    std::vector<Node*> headers(cols);

    Node* predecessor = root;
    for (size_t i = 0; i < cols; ++i) {
        headers[i] = new Node;
        headers[i]->up = headers[i];
        headers[i]->down = headers[i];
        headers[i]->left = predecessor;
        headers[i]->right = root;
        headers[i]->data = 0;

        // Update immediate neighbors.
        root->left = headers[i];
        predecessor->right = headers[i];

        // Update predecessor for next iteration.
        predecessor = headers[i];
    }

    return headers;
}

// Allocate and initialize cover matrix row headers.
std::vector<Node*> build_row_headers(Node* root, size_t rows) {
    std::vector<Node*> headers(rows);

    Node* predecessor = root;
    for (size_t i = 0; i < rows; i++) {
        headers[i] = new Node;
        headers[i]->up = predecessor;
        headers[i]->down = root;
        headers[i]->left = headers[i];
        headers[i]->right = headers[i];

        // Update immediate neighbors.
        root->up = headers[i];
        predecessor->down = headers[i];

        // Update predecessor for next iteration.
        predecessor = headers[i];
    }

    return headers;
}

// Remove and deallocate row headers from the newly
// constructed cover matrix.
void delete_row_headers(std::vector<Node*>& headers) {
    typedef std::vector<Node*>::size_type size_type;
    for (size_type i = 0; i < headers.size(); i++) {
        Node* header = headers[i];
        header->left->right = header->right;
        header->right->left = header->left;
        header->up->down = header->down;
        header->down->up = header->up;
        delete header;
    }
}

// Given the root node, deallocate the cover matrix.
void delete_cover_matrix(Node* root) {
    Node* header = root->right;
    Node* old, *node;

    // Go over each header of the cover matrix.
    while (header != root) {
        node = header->down;

        // Deallocate each element in the
        // column associated to this header.
        while (node != header) {
            old = node;
            node = node->down;
            delete old;
        }

        // Deallocate the header.
        old = header;
        header = header->right;
        delete old;
    }

    delete root;
}

// Choose the next column to cover based on some heuristic,
// e.g. the number of elements contained in a column.
Node* choose_next_column(Node* root)  {
    uint32 lower = std::numeric_limits<uint32>::max();

    Node* current = root->right;
    Node* next = current;

    while (current != root) {
        if (current->data < lower) {
            lower = current->data;
            next = current;
        }
        current = current->right;
    }

    return next;
}

// Given an header node, cover the associated column.
void cover_column(Node* header) {
    header->left->right = header->right;
    header->right->left = header->left;

    Node* col = header->down;
    while (col != header) {
        Node* row = col->right;
        while (row != col) {
            row->up->down = row->down;
            row->down->up = row->up;
            row->header->data--;
            row = row->right;
        }
        col = col->down;
    }
}

// Given an header node, uncover the associated column.
void uncover_column(Node* header) {
    Node* col = header->up;

    while (col != header) {
        Node* row = col->left;
        while (row != col) {
            row->up->down = row;
            row->down->up = row;
            row->header->data++;
            row = row->left;
        }
        col = col->up;
    }

    header->left->right = header;
    header->right->left = header;
}

// The actual recursive procedure implementing the dancing links
// algorithm. Indexes of the rows forming the exact cover will
// be appended to the cover vector if a solution is found.
bool solve(Node* root, std::vector<uint32>& cover) {
    bool solved = false;
    Node* header = choose_next_column(root);

    if (header == root) {
        return true;
    }

    cover_column(header);

    Node* column_element = header->down;
    while (column_element != header) {
        Node* row_element = column_element->right;
        while (row_element != column_element) {
            cover_column(row_element->header);
            row_element = row_element->right;
        }

        solved = solve(root, cover);

        row_element = column_element->left;
        while (row_element != column_element) {
            uncover_column(row_element->header);
            row_element = row_element->left;
        }

        // If we've solved the exact cover problem,
        // set the cell value accordingly.
        if (solved) {
            cover.push_back(column_element->data);
            break;
        }

        column_element = column_element->down;
    }

    uncover_column(header);
    return solved;
}

// Allocate and initialize the cover matrix for a given binary
// matrix. The cover matrix should be deallocated with the
// delete_cover_matrix(Node*) function.
template <typename Matrix>
Node* build_cover_matrix(const Matrix& matrix)  {
    Node* root = build_root();

    // These vectors will be used to get fast access to row and
    // column headers during the initialization phase of the cover
    // matrix. Row headers will immediately be deleted at the
    // end of the initialization process.
    std::vector<Node*> row_headers = build_row_headers(root, matrix.rows());
    std::vector<Node*> column_headers = build_column_headers(root, matrix.cols());

    for (size_t row = 0; row < matrix.rows(); row++) {
        Node* row_first = row_headers[row];
        Node* row_last = row_headers[row];

        for (size_t col = 0; col < matrix.cols(); col++) {
            if (matrix(row, col)) {
                Node* header = column_headers[col];

                // Initialize new node.
                Node* node = new Node;
                node->header = header;
                node->up = header->up;
                node->down = header;
                node->left = row_last;
                node->right = row_first;
                node->data = row;

                // Update neighbors.
                node->up->down = node;
                node->left->right = node;

                header->up = node;
                header->data++;

                row_first->left = node;
                row_last = node;
            }
        }
    }

    delete_row_headers(row_headers);

    return root;
}

} // namespace details

// Solve an exact cover instance encoded into a binary matrix.
// Indexes of the rows forming the exact cover are returned in
// a vector. If no solution was found, an empty vector is returned.
template <typename Matrix>
std::vector<uint32> solve(const Matrix& matrix) {
    std::vector<uint32> cover;
    details::Node* root;

    root = details::build_cover_matrix(matrix);
    details::solve(root, cover);
    details::delete_cover_matrix(root);

    return cover;
}

} // namespace exact_cover

#endif // EXACT_COVER_SOLVER_HPP_
