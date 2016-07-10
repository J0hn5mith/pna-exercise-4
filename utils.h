struct Timer{
    double start_time;
    double end_time;
    double  duration;
};

void start_timer(struct Timer* timer);
void stop_timer(struct Timer* timer);
