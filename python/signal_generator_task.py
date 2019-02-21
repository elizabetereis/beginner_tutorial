import time
import threading

from hyro.core_interface import *
from hyro.core import *
from hyro.utils import *
from hyro.common import *
from hyro.task import *
from hyro.signal_generator import *


class SignalGeneratorTask(Task):

  def __init__(self, uri):
    self.uri = uri

    self.cmp_signalgenerator = Task.createComponent(SignalGeneratorComponent, self.uri, "signal_in")
    self.cmp_digitalconverter = Task.createComponent(DigitalConverterComponent, self.uri, "signal_out")

    self.components = [self.cmp_signalgenerator, self.cmp_digitalconverter]
    self._thread = None
 
  def reset(self):
    raise NotImplementedError("Missing method reset()")

  def switchOn(self):
    return Task.switchOnAll(self.components)

  def init(self):
    config_map = {}
    config_map[self.cmp_signalgenerator] = """ 
    outputs: 
        signals: 
            protocol: 'api' 
    """
    config_map[self.cmp_digitalconverter] = """
    inputs: 
        signals: 
            protocol: 'api'
    outputs: 
        digital_signal: 
            protocol: 'api'
    """
    return Task.initAll(self.components, config_map)
    
  def _doWork(self):
    self.cmp_signalgenerator.update()
    self.cmp_digitalconverter.update()

  def start(self, finished_event):
    if Task.connectComponent(self.cmp_digitalconverter, "signals", self.cmp_signalgenerator, "signals") != Result.Ok:
      return Result.Error
    if Task.startAll(self.components) != Result.Ok:
      return Result.Error

    self.index = 0
    self._thread = RatedThread()
    self._thread.start(rate = 0.01, finished_event = finished_event, target = self._doWork, finish_condition = lambda : self.cmp_signalgenerator.finished())
    return Result.Ok
  

  def stop(self):
    self._thread.stop()
    self._thread.join()
    return Result.Ok

  def suspend(self):
    self._thread.suspend()
    return Task.suspendAll(self.components)

  def resume(self):
    result = Task.resumeAll(self.components)
    self._thread.resume()
    return result

  def abort(self):
    raise NotImplementedError("Missing method abort()")
    
  def checkFinished(event):
    event.set()

if __name__ == "__main__":

  HyroLoggerManager.SetLevel(LogLevel.Trace);
  HyroLoggerManager.SetLevel("AvahiDiscoveryService", LogLevel.Warning);
  HyroLoggerManager.SetLevel("ConfigurationRegistry", LogLevel.Warning);

  if False:
    finish_event = threading.Event()
    task_sm = TaskStateMachine(SignalGeneratorTask, "/signal_generator_task")

    task_sm.init()
    task_sm.start()
    task_sm.finished_event.append(lambda : checkFinished(finish_event))

    time.sleep(10)

    task_sm.switchOn()
    task_sm.stopDispatcher()