#ifndef ENTRY_H
#define ENTRY_H


class Entry{
    private:
        String category;
        double x, y;
        String destinaiton;
        double angle;
    public:
        Entry(String package);
        double getDistance();
};
#endif