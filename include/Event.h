#ifndef PICMDK_EVENT_H
#define PICMDK_EVENT_H


namespace picmdk {
    

// Base class for all events used at handlers
class Event {
public:
    enum Type { IterationStart, ParticlePostPush, Cell, Output, numEvents };

    virtual ~Event() {}
    virtual Type getType() const = 0;
};

template<class Controller>
class IterationStartEvent : public Event {
public:
    typedef typename Controller::Ensemble Ensemble;
    typedef typename Controller::Grid Grid;

    IterationStartEvent(Ensemble& _ensemble, Grid& _grid) :
        ensemble(_ensemble),
        grid(_grid)
    {
    }

    virtual Event::Type getType() const { return Event::IterationStart; }

    Ensemble& getEnsemble() { return ensemble; }
    Grid& getGrid() { return grid; }

private:

    Ensemble& ensemble;
    Grid& grid;
};


template<class Controller>
class ParticlePostPushEvent : public Event {
public:
    typedef typename Controller::Ensemble Ensemble;
    typedef typename Controller::Grid Grid;
    typedef typename Controller::Particle Particle;

    ParticlePostPushEvent(Particle& _particle, Ensemble& _ensemble, Grid& _grid):
        particle(_particle),
        ensemble(_ensemble),
        grid(_grid)
    {
    }

    virtual Event::Type getType() const { return Event::PostParticlePush; }

    Particle& getParticle() { return particle; }
    Ensemble& getEnsemble() { return ensemble; }
    Grid& getGrid() { return grid; }

private:

    Particle& particle;
    Ensemble& ensemble;
    Grid& grid;
};


template<class Controller>
class CellEvent : public Event {
public:
    typedef typename Controller::Cell Cell;
    typedef typename Controller::Grid Grid;

    CellEvent(Cell& _cell, Grid& _grid) :
        cell(_cell),
        grid(_grid)
    {
    }

    virtual Event::Type getType() const { return Event::Cell; }

    Cell& getCell() { return cell; }
    Grid& getGrid() { return grid; }

private:

    Cell& cell;
    Grid& grid;
};


template<class Controller>
class OutputEvent : public Event {
public:
    virtual Event::Type getType() const { return Event::Output; }
};


} // namespace picmdk

#endif
