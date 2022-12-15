#include "myedgeset.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class EdgeSetTest : public ::testing::Test {
 public:
  EdgeSetTest() : edges_(nullptr), eset_(nullptr) {}
  ~EdgeSetTest() {
    delete[] edges_;
    delete eset_;
  }

  void MakeEdgeSet(int n) {
    delete eset_;
    delete[] edges_;
    model_.clear();
    edges_ = new Edge[n];
    eset_ = new EdgeSet;

    for (int i = 0; i < n; ++i) {
      eset_->insert(&edges_[i]);
      model_.insert(&edges_[i]);
    }
  }

  void CheckSame() {
    EXPECT_EQ(model_.size(), eset_->size());
    EXPECT_EQ(model_.empty(), eset_->empty());
    std::vector<const Edge*> esetv(eset_->begin(), eset_->end());
    std::vector<const Edge*> modelv(model_.begin(), model_.end());
    std::sort(modelv.begin(), modelv.end());
    std::sort(esetv.begin(), esetv.end());
    
    EXPECT_EQ(modelv.size(), esetv.size());

    for (size_t i = 0; i < modelv.size(); ++i) {
      EXPECT_EQ(modelv[i], esetv[i]) << i;
    }
  }

  Edge* edges_;
  EdgeSet* eset_;
  std::set<const Edge*> model_;
};

TEST_F(EdgeSetTest, Ops) {
  for (int n: {0,1,2,3,4,10}) {
    MakeEdgeSet(n);
    CheckSame();
    EXPECT_EQ((n == 0), eset_->empty());
    EXPECT_EQ(n, eset_->size());

    eset_->clear();
    model_.clear();
    CheckSame();

    eset_->insert(&edges_[0]);
    model_.insert(&edges_[0]);

    CheckSame();
  }
}