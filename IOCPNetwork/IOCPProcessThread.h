#pragma once

#define  THREAD_DIE 1

interface IIOCPProcessThread
{
	virtual void ProcessingThread() = 0;
};