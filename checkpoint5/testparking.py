import threading

def MakeParkingLot(N):
    global sem        # semaphore for the parking lot
    global spots      # list for the spots
    global spotsSync  # for synchronizing access to spots
    spots = [None for i in range(N)]
    #  your code to initialize sem and spotsSync 
    sem = threading.BoundedSemaphore(1)
    spotsSync = threading.Semaphore(N)

def MakeCars(C):
    # your code here to spawn threads
    # don’t forget to return the list
    cars = [threading.Thread(target=Park, args=(i,)) for i in range(C)]
    return cars

def Park(car):
    global sem, spots, spotsSync
    # 2.3.1  ############################
    # if spot available, grab it; otherwise wait until available.
    #  Hint: don’t use if/else statement!  this is just one line.
    spotsSync.acquire()
    # 2.3.2 ###########################################
    # after confirming one parking spot, modify the spots 
    # (Python list or your choice of list-like data structure to
    # put this car into the spot.  The following is an example
    # of what it can do, but you may have a different way of
    # grabbing parking spots.
    # Do you need to protect access to the following block of
    # code? If so, add code to protect it; if not, why not?
    for i in range(len(spots)):
        if spots[i] is None:
            sem.acquire()
            spots[i] = car
            sem.release()
            break
    # sem.release()
    snapshot = spots[:]  # make a copy for printing
    # now let us print out the current occupancy
    print("Car %d got spot: %s" % (car, snapshot))
    # leave the car on the lot for some (real) time!
    import time
    import random
    st = random.randrange(1,10)
    time.sleep(st)
    # now ready to exit the parking lot.  What do we need to 
    # 2.3.3 ################################
    # update the spots data structure by replacing the spot 
    #    that current car occupies with the value None; 
    #    protect code if needed
    # (2) print out the status of the spots
    for i in range(len(spots)):
        if spots[i] == car:
            sem.acquire()
            spots[i] = None
            sem.release()
    myCopySpots = spots[:]
    print("Car %d left after %d sec, %s" % (car, st, myCopySpots))
    # 2.3.4 #################################
    # (3) give the spot back to the pool 
    # (hint: semaphore operation)
    spotsSync.release()

if __name__ == '__main__':
    MakeParkingLot(5)
    cars = MakeCars(15)
    for i in range(15): 
        cars[i].start()