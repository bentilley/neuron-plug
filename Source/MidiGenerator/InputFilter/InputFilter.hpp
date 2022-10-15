/*
 * InputFilter.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../Brain/io.hpp"
#include <map>
#include <memory>
#include <vector>

class InputFilter {
public:
  /** The strategy used to merge input vectors from multiple input streams.
   *
   * - **Simple Merge**: Only merge vectors if they have exactly the same sample
   *   number (i.e. the naieve strategy). This is the default merge strategy.
   * - **N Forward Sample Merge**: After a vector is included, also merge in any
   *   vectors that are within `N` samples of the first vector.
   *
   * @see SimpleMergeStrategy
   * */
  enum class MergeStrategy : std::uint_least8_t {
    SimpleMerge,        ///< Only merge vectors with the same sample number.
    ForwardSampleMerge, ///< Merge vectors that closely follow the first vector.
  };

  /** Implementation of a strategy for merging input vectors.
   *
   * This will be called by the InputFilter for each input vector with the
   * current merged vectors and the next vector to be merged.
   */
  class MergeStrategyImplementation {
  public:
    virtual ~MergeStrategyImplementation(){};

    /** How this strategy merges a vector into the currently merged vectors.
     *
     * This is a virtual method defines the behaviour of this
     * MergeStrategyImplementation. This will be called by the InputFilter to
     * determine how to merge in each input vector to the merged output.
     *
     * @param mergedVectors A vector of all of the currently mergedVectors.
     * @param nextVector The next ModelVector vector to be merged.
     */
    virtual void mergeInputVector(
      std::vector<ModelVector>& mergedVectors,
      ModelVector& nextVector
    ) = 0;
  };

  /** Default constructor creates an InputFilter that uses the
   * SimpleMergeStrategy.
   */
  InputFilter();
  /** A constructor that allows you to choose the initial MergeStrategy that the
   * InputFilter uses.
   *
   * @param strategy The enum of the MergeStrategy for the InputFilter to use.
   */
  InputFilter(MergeStrategy strategy);

  /** Merge the different input streams using the configured strategy.
   *
   * The behaviour of this method is governed by the implementation of the
   * specific strategy being used.
   * @see InputFilter::MergeStrategyImplementation
   *
   * @param streams The input streams to be merged.
   */
  std::vector<ModelVector> mergeInputStreams(std::vector<std::vector<ModelVector>>& streams) const;

  /** Get the enum of the currently configured MergeStrategy. */
  MergeStrategy getMergeStrategy() const;

  /** Set the MergeStrategy that this InputFilter will use.
   *
   * This sets both the configuration enum, and the InputFilter::strategy
   * pointer to the InputFilter::MergeStrategyImplementation being used.
   *
   * @param newStrategy The new strategy for the InputFilter to use.
   */
  void setMergeStrategy(MergeStrategy newStrategy);

private:
  MergeStrategy mergeStrategy; ///< The InputFilter::MergeStrategy being used.
  std::shared_ptr<MergeStrategyImplementation>
    strategy; ///< The corresponding InputFilter::MergeStrategyImplementation
};

/** A naieve merge strategy that simply merges input vectors if they have the
 * same sample number.
 *
 * @see InputFilter::MergeStrategyImplementation
 */
class SimpleMergeStrategy : public InputFilter::MergeStrategyImplementation {
public:
  void mergeInputVector(std::vector<ModelVector>& mergedVectors, ModelVector& nextVector) override;
};
