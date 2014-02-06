#include "ParallelTask.hpp"

ParallelTask::ParallelTask()
	:mThread(&ParallelTask::runTask, this),
	mFinished(false)
{
}

void ParallelTask::execute()
{
	mFinished = false;
	mElapsedTime.restart();
	mThread.launch();
}

float ParallelTask::getCompletion()
{
	sf::Lock lock(mMutex);
	
	// 100% at 2 seconds of elapsed time
	return mElapsedTime.getElapsedTime().asSeconds() / 2.f;
}

bool ParallelTask::isFinished()
{
	sf::Lock lock(mMutex);
	return mFinished;
}

void ParallelTask::runTask()
{
	// Dummy task - stall 2 seconds
	bool ended = false;
	while(!ended)
	{
		sf::Lock lock(mMutex);
		if(mElapsedTime.getElapsedTime().asSeconds() >= 2.f)
			ended = true;
	}
	
	{
		sf::Lock lock(mMutex);
		mFinished = true;
	}
}
