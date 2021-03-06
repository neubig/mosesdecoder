/***********************************************************************
Moses - factored phrase-based language decoder
Copyright (C) 2006 University of Edinburgh

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
***********************************************************************/

#pragma once

#include "ChartTranslationOptions.h"
#include "ChartParserCallback.h"
#include "StackVec.h"

#include <vector>

namespace Moses
{

class TargetPhraseCollection;
class WordsRange;

//! a vector of translations options for a specific range, in a specific sentence
class ChartTranslationOptionList : public ChartParserCallback
{
public:
  ChartTranslationOptionList(size_t);
  ~ChartTranslationOptionList();

  const ChartTranslationOptions &Get(size_t i) const {
    return *m_collection[i];
  }

  //! number of translation options
  size_t GetSize() const {
    return m_size;
  }

  void Add(const TargetPhraseCollection &, const StackVec &,
           const WordsRange &);

  void AddPhraseOOV(TargetPhrase &phrase, std::list<TargetPhraseCollection*> &waste_memory, const WordsRange &range);

  bool Empty() const {
    return m_size == 0;
  }

  void Clear();
  void ApplyThreshold();

private:
  typedef std::vector<ChartTranslationOptions*> CollType;

  struct ScoreThresholdPred {
    ScoreThresholdPred(float threshold) : m_thresholdScore(threshold) {}
    bool operator()(const ChartTranslationOptions *option)  {
      return option->GetEstimateOfBestScore() >= m_thresholdScore;
    }
    float m_thresholdScore;
  };

  CollType m_collection;
  size_t m_size;
  float m_scoreThreshold;
  const size_t m_ruleLimit;
};

}
