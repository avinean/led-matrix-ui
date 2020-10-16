class timerMinim
{
  public:
    timerMinim(uint32_t interval);        // объявление таймера с указанием интервала
    void setInterval(uint32_t interval);  // установка интервала работы таймера
    boolean isReady();                    // возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
    void reset();                         // ручной сброс таймера на установленный интервал
    void pause(bool _do);                         // ручной сброс таймера на установленный интервал

  private:
    bool _paused = false;
    uint32_t _timer = 0;
    uint32_t _interval = 0;
};


timerMinim::timerMinim(uint32_t interval) {
  _interval = interval;
  _timer = millis();
}

void timerMinim::pause(bool _do) {
  _paused = _do;
}


void timerMinim::setInterval(uint32_t interval) {
  _interval = interval;
}

boolean timerMinim::isReady() {
  if ( !_paused && ( (long)millis() - _timer >= _interval ) ) {
    _timer = millis();
    return true;
  } else {
    return false;
  }
}

void timerMinim::reset() {
  _timer = millis();
}
