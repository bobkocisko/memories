//
// Created by bob on 4/23/17.
//

#pragma once

#include <memory>

class EventSource {
 public:
  virtual ~EventSource() {}

  enum ReadyTypes {
    ReadyToRead = 1,
    ReadyToWrite = 2
  };

  virtual void NotifyIsReady(ReadyTypes readyTypes) = 0;
};
