//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// properties.h
//
// Identification: src/include/optimizer/properties.h
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include "optimizer/column.h"
#include "expression/tuple_value_expression.h"
#include "expression/expression_util.h"

namespace peloton {
namespace optimizer {

// Specifies which columns should the executor return
class PropertyColumns : public Property {
 public:
  PropertyColumns(std::vector<expression::AbstractExpression *> column_exprs);

  PropertyType Type() const override;

  hash_t Hash() const override;

  bool operator>=(const Property &r) const override;

  void Accept(PropertyVisitor *v) const override;
  
  std::string ToString() const override;

  inline expression::AbstractExpression *GetColumn(int idx) const {
    return column_exprs_[idx];
  }

  inline size_t GetSize() const { return column_exprs_.size(); }
//  inline bool IsStarExpressionInColumn() const { return is_star_; }

 private:
  std::vector<expression::AbstractExpression *> column_exprs_;
};

// Specify which columns values are dictinct
// PropertyDistinct(col_a, col_b, col_c) should have 
// distinct value for (col_a, col_b, col_c) in each return tuple
class PropertyDistinct : public Property {
 public:
  PropertyDistinct(std::vector<expression::AbstractExpression *> column_exprs,
                  bool is_star = false);
  PropertyDistinct(bool is_star_expr);

  PropertyType Type() const override;

  hash_t Hash() const override;

  bool operator>=(const Property &r) const override;

  void Accept(PropertyVisitor *v) const override;
  
  std::string ToString() const override;

  inline expression::AbstractExpression *GetDistinctColumn(int idx) const {
    return distinct_column_exprs_[idx];
  }

  inline size_t GetSize() const { return distinct_column_exprs_.size(); }
  inline bool IsStarExpressionInColumn() const { return is_star_; }

 private:
  std::vector<expression::AbstractExpression *> distinct_column_exprs_;
  bool is_star_;
};

// Specifies the output expressions of the query
class PropertyProjection : public Property {
 public:
  PropertyProjection(
      std::vector<std::unique_ptr<expression::AbstractExpression>> expressions);

  PropertyType Type() const override;

  hash_t Hash() const override;

  bool operator>=(const Property &r) const override;

  void Accept(PropertyVisitor *v) const override;
  
  std::string ToString() const override;

  inline size_t GetProjectionListSize() const { return expressions_.size(); }

  inline expression::AbstractExpression *GetProjection(size_t idx) const {
    return expressions_[idx].get();
  }

  inline bool AllAggregation() const {
    for (size_t idx = 0; idx < expressions_.size(); idx++)
      if (!expression::ExpressionUtil::IsAggregateExpression
          (expressions_[idx]->GetExpressionType()))
        return false;
    return true;
  }

 private:
  std::vector<std::unique_ptr<expression::AbstractExpression>> expressions_;
};

// Specifies the required sorting order of the query
class PropertySort : public Property {
 public:
  PropertySort(std::vector<expression::AbstractExpression *> sort_columns,
               std::vector<bool> sort_ascending);

  PropertyType Type() const override;

  hash_t Hash() const override;

  bool operator>=(const Property &r) const override;

  void Accept(PropertyVisitor *v) const override;
  
  std::string ToString() const override;

  inline size_t GetSortColumnSize() const { return sort_columns_.size(); }

  inline expression::AbstractExpression *GetSortColumn(int idx) const {
    return sort_columns_[idx];
  }

  inline bool GetSortAscending(int idx) const { return sort_ascending_[idx]; }

 private:
  std::vector<expression::AbstractExpression *> sort_columns_;
  std::vector<bool> sort_ascending_;
};

// Specifies the predicate that the tuples returned by the query should satisfy
class PropertyPredicate : public Property {
 public:
  PropertyPredicate(expression::AbstractExpression *predicate);

  PropertyType Type() const override;

  hash_t Hash() const override;

  bool operator>=(const Property &r) const override;

  void Accept(PropertyVisitor *v) const override;
  
  std::string ToString() const override;

  inline expression::AbstractExpression *GetPredicate() const {
    return predicate_.get();
  }

 private:
  std::unique_ptr<expression::AbstractExpression> predicate_;
};

} /* namespace optimizer */
} /* namespace peloton */
