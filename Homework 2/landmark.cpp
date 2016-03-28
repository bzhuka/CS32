/*#include <iostream>
#include <string>
using namespace std;
*/
//Your declarations and implementations would go here
class Landmark {
public:
    Landmark(string name) {
        m_name = name;
    }
    virtual ~Landmark() {
        cout << m_name << "." << endl;
    }
    string name() const{
        return m_name;
    }
    virtual string color() const{
        return "yellow";
    }
    virtual string icon() const{
        return m_icon;
    }
private:
    string m_name;
    string m_icon;
};

class Hotel:public Landmark {
public:
    Hotel(string name): Landmark(name) {
        m_icon = "bed";
    }
    virtual ~Hotel() {
        cout << "Destroying the hotel ";
    }
    virtual string icon() const{
        return m_icon;
    }
private:
    string m_icon;
};

class Restaurant: public Landmark {
public:
    Restaurant(string name, int cap): Landmark(name) {
        if (cap < 40) {
            m_icon = "small knife/fork";
        } else {
            m_icon = "large knife/fork";
        }
    }
    virtual ~Restaurant() {
        cout << "Destroying the restaurant ";
    }
    virtual string icon() const{
        return m_icon + Landmark::icon();
    }
private:
    string m_icon;
};

class Hospital: public Landmark {
public:
    Hospital(string name): Landmark(name) {
        m_icon = "H";
    }
    virtual ~Hospital() {
        cout << "Destroying the hospital ";
    }
    virtual string color() const {
        return "blue";
    }
    virtual string icon() const{
        return m_icon;
    }
private:
    string m_icon;
};
/*
void display(const Landmark* lm)
{
    cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
    << lm->name() << "." << endl;
}

int main()
{
    Landmark* landmarks[4];
    landmarks[0] = new Hotel("Westwood Rest Good");
    // Restaurants have a name and seating capacity.  Restaurants with a
    // capacity under 40 have a small knife/fork icon; those with a capacity
    // 40 or over have a large knife/fork icon.
    landmarks[1] = new Restaurant("Bruin Bite", 30);
    landmarks[2] = new Restaurant("La Morsure de l'Ours", 100);
    landmarks[3] = new Hospital("UCLA Medical Center");
    
    cout << "Here are the landmarks." << endl;
    for (int k = 0; k < 4; k++)
        display(landmarks[k]);
    
    // Clean up the landmarks before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete landmarks[k];
}*/