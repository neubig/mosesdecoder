#include "FeatureConsumer.h"
#include "Util.h"
#include "FeatureConsumer.h"
#include <stdexcept>
#include <exception>
#include <string>

using namespace std;
using namespace Moses;

VWFileTrainConsumer::VWFileTrainConsumer(const std::string &outputFile)
{
  m_os.open(outputFile.c_str());
  if (! m_os.good())
    throw runtime_error("Cannot write into file: " + outputFile);
}

void VWFileTrainConsumer::SetNamespace(char ns, bool shared)
{
  if (! m_outputBuffer.empty())
    WriteBuffer();

  if (shared)
    m_outputBuffer.push_back("shared");

  m_outputBuffer.push_back("|" + ns);
}

void VWFileTrainConsumer::AddFeature(const std::string &name)
{
  m_outputBuffer.push_back(name);
}

void VWFileTrainConsumer::AddFeature(const std::string &name, float value)
{
  m_outputBuffer.push_back(name + ":" + SPrint(value));
}

void VWFileTrainConsumer::FinishExample()
{
  WriteBuffer();
  m_os << endl;
}

void VWFileTrainConsumer::Finish()
{
  m_os.close();
}

void VWFileTrainConsumer::Train(const std::string &label, float loss)
{
  m_outputBuffer.push_front(label + ":" + SPrint(loss));
}

float VWFileTrainConsumer::Predict(const std::string &label) 
{
  throw logic_error("Trying to predict during training!");
}

void VWFileTrainConsumer::WriteBuffer()
{
  m_os << Join(" ", m_outputBuffer.begin(), m_outputBuffer.end()) << endl;
  m_outputBuffer.clear();
}