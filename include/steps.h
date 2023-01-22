#ifndef steps_h
#define steps_h

class steps {
    public:
    steps();
    void run();
    int getSteps() {
        return m_steps;
    };
    private:
    int m_steps;
};

#endif //steps_h