import geneticalgo
import random
import copy
import os
import json
import sys
import math
from geneticalgo import algorithm
from geneticalgo import primitives
from geneticalgo import primitives2D
from geneticalgo import piecewisePolynom2D
from sofalauncher import launcher

mutationType="OFF"
mutationThickness="OFF"
mutationCoef="OFF"
mutationWidth="ON"
mutationDepht="OFF"
mutationPosition="OFF"

def getNextId():
    global individualId
    individualId+=1
    return individualId

def getJSONFragmentFrom(file):

    with open(file, "r") as source:
        for line in source:
            if "JSON : " in line:
                data = json.loads(line[7:])
                return data

    return None




class CrochetIndividual(algorithm.Individual):

    def __init__(self):
        self.level=None
        self.id = getNextId()
        crochetutils.create(self)
        


def newIndividualFrom(ind):
    newInd=CrochetIndividual()
    newInd.listCavities=copy.deepcopy(ind.listOfDrawnPoints)

    return newInd

def getShapeFromInd(ind):
    return crochet.generateCrochetManually(ind.listOfDrawnPoints)



####################################################################################################
### MUTATION
###



def mutation_Position(ind, side):

    length=len(ind.listOfDrawnPoints)
    if length < 2:
        raise ValueError, "don't touch the bottom and the top!"

    if not side in ["left", "right"]:
        raise ValueError, "choose a side"

    ind.listOfDrawnPoints[index][0]+=random.uniform(-0.0001,0.0001)
    ind.listOfDrawnPoints[index][1]+=random.uniform(-0.0001,0.0001)





def mutation_Depht(ind, side):

    length=len(ind.listOfDrawnPoints)
    if length < 2:
        raise ValueError, "don't touch the bottom and the top!"

    if not side in ["left", "right"]:
        raise ValueError, "choose a side"


    index=random.randint(1,length-2)

    if side = "left":

        param = ind.listOfDrawnPoints[index][2]

        if param[0]=="corner":
            Depht=param[4]
            epsilon = random.uniform(0.8,1.2)
            ind.listOfDrawnPoints[index][2][4]*=epsilon

    else:

        param = ind.listOfDrawnPoints[index][3]

        if param[0]=="corner":
            Depht=param[4]
            epsilon = random.uniform(0.8,1.2)
            ind.listOfDrawnPoints[index][3][4]*=epsilon

def mutation_Width(ind, side):

    length=len(ind.listOfDrawnPoints)
    if length < 2:
        raise ValueError, "don't touch the bottom and the top!"

    if not side in ["left", "right"]:
        raise ValueError, "choose a side"


    index=random.randint(1,length-2)

    if side = "left":

        param = ind.listOfDrawnPoints[index][2]

        if param[0]=="corner":
            width=param[3]
            epsilon = random.uniform(0.8,1.2)
            ind.listOfDrawnPoints[index][2][3]*=epsilon

    else:

        param = ind.listOfDrawnPoints[index][3]

        if param[0]=="corner":
            width=param[3]
            epsilon = random.uniform(0.8,1.2)
            ind.listOfDrawnPoints[index][3][3]*=epsilon

def mutation_Type(ind, side):

    length=len(ind.listOfDrawnPoints)
    if length < 2:
        raise ValueError, "don't touch the bottom and the top!"

    if not side in ["left", "right"]:
        raise ValueError, "choose a side"

    index=random.randint(1,length-2)

    if side = "left":

        param = ind.listOfDrawnPoints[index][2]
        type=param[0]

        if type == "smooth":

            type = "corner"
            ind.listOfDrawnPoints[index][2][0] = type
            ind.listOfDrawnPoints[index][2].append(0.0007)

            ind.listOfDrawnPoints[index][2].append(0.0007)

        else:
            type = "smooth"
            del ind.listOfDrawnPoints[index][2][-2,-1]

    else:

        param = ind.listOfDrawnPoints[index][3]
        type=param[0]

        if type == "smooth":

            type = "corner"
            ind.listOfDrawnPoints[index][3][0] = type
            ind.listOfDrawnPoints[index][3].append(0.0007)
            ind.listOfDrawnPoints[index][3].append(0.0007)

        else:
            type = "smooth"
            del ind.listOfDrawnPoints[index][3][-2,-1]


def mutation_Thickness(ind, side):

    length=len(ind.listOfDrawnPoints)

    if not side in ["left", "right"]:
        raise ValueError, "choose a side"

    index=random.randint(0,length-1)
    if side = "left":

        param = ind.listOfDrawnPoints[index][2]
    else:
        param = ind.listOfDrawnPoints[index][3]


    thickness=param[1]
    epsilon=random.uniform(thickness/8.0,thickness/4.0)
    thickness+=epsilon
    if side = "left":

        ind.listOfDrawnPoints[index][2][1] = thickness
    else:

        ind.listOfDrawnPoints[index][3][1] = thickness



def mutation_Coef(ind, side):

    length=len(ind.listOfDrawnPoints)
    if length < 2:
        raise ValueError, "don't touch the bottom and the top!"

    if not side in ["left", "right"]:
        raise ValueError, "choose a side"

    index=random.randint(1,length-2)
    if side = "left":

        param = ind.listOfDrawnPoints[index][2]
    else:
        param = ind.listOfDrawnPoints[index][3]


    coef = param[2]
    epsilon=random.uniform(0.8,1.2)
    coef*=epsilon
    if side = "left":

        ind.listOfDrawnPoints[index][2][2] = coef
    else:

        ind.listOfDrawnPoints[index][3][2] = coef


mutationType="OFF"
mutationThickness="OFF"
mutationCoef="OFF"
mutationWidth="ON"
mutationDepht="OFF"
mutationPosition="OFF"

def mutation(ind):

    if mutationType=="ON":
        if random.choice([True, False]):
            side = random.choice("left", "right")
            mutation_Type(ind, side)

    if mutationThickness=="ON":

        if random.choice([True, False]):

            side = random.choice("left", "right")
            mutation_Thickness(ind, side)


    if mutationCoef=="ON":

        if random.choice([True, False]):
            side = random.choice("left", "right")
            mutation_Coef(ind, side)

    if mutationWidth=="ON":
        if random.choice([True, False]):
            side = random.choice("left", "right")
            mutation_Width(ind, side)

    if mutationDepht=="ON":

        if random.choice([True, False]):

            side = random.choice("left", "right")
            mutation_Depht(ind, side)


    if mutationPosition=="ON":

        if random.choice([True, False]):
            side = random.choice("left", "right")
            mutation_Position="OFF"(ind, side)


def mutationFunc(pop, params):

    number_of_mutated_ind=params["nbIndMutated"]
    number_of_mutation_per_ind=params["nbMutationsPerInd"]

    if not (isinstance(number_of_mutated_ind, int) and isinstance(number_of_mutation_per_ind, int)):
        raise ValueError, "nbIndMutated and nbMutationsPerInd are not given or are not int"

    length_temp=len(pop)

    for i in range(number_of_mutated_ind):
        j=random.randint(0,length_temp-1)
        ind=newIndividualFrom(pop[j])

        for k in range(number_of_mutation_per_ind):
            mutation(ind)

        pop.append(ind)

    return pop

####################################################################################################
###CROSSING
###
def crossing_ind(individual1, individual2):

    length1=len(individual1.listCavities)

    if length1!=len(individual2.listCavities):

        raise ValueError, "they have not the same number of cavities"

    ind1=newIndividualFrom(individual1)
    ind2=newIndividualFrom(individual2)
    index=random.randint(0,length1-1)

    temp1 = ind1.listCavities[index]
    temp2 = ind2.listCavities[index]

    ind1.listCavities[index]=temp2
    ind2.listCavities[index]=temp1

    return (ind1, ind2)


def crossFunc(pop, params):

    number_of_crossing=params["crossTx"]

    if not isinstance(number_of_crossing, int):
        raise ValueError, "crossTx is not given or is not an int"
    print("crossFunc "+str(pop))

    length_temp=len(pop)

    newpop = algorithm.Population()
    for i in range(number_of_crossing):
        j=random.randint(0, length_temp-1)
        k=random.randint(0, length_temp-1)
        (a,b)=crossing_ind(pop[j], pop[k])
        newpop.append(a)
        newpop.append(b)
    return newpop

####################################################################################################
### Generate
###
def generateIndividual(aType):
        individual=AccordionIndividual()

        for i in range(1,number_of_cavities+1):
            height=0.5+i*(individual.height-1.0)/float(number_of_cavities+1)

            if aType=="ellipsoid":
                if generate_random=="ON":
                    axisX=max(individual.thickness+1.0, random.uniform((4.0/3.0)*individual.radius,(10.0/3.0)*individual.radius))
                    axisY=max(individual.thickness+1.0, random.uniform(2.0*individual.thickness,(10.0/3.0)*individual.radius))
                    axisZ=max(individual.thickness+1.0, (individual.height-1.0)/float((2*(number_of_cavities+1))))

                else:
                    axisX=max(individual.height/2.5 , max(individual.thickness+1.0, (7.0/3.0)*individual.radius))
                    axisY=max(individual.height/2.5 , max(individual.thickness+1.0, (7.0/3.0)*individual.radius))
                    axisZ=max(individual.thickness+1.0, (individual.height-1.0)/float((2*(number_of_cavities+1))))

            else:
                if generate_random=="ON":
                    axisX=max(individual.thickness+1.0, random.uniform((4.0/3.0)*individual.radius,(10.0/3.0)*individual.radius))
                    axisY=max(individual.thickness+1.0, random.uniform(2.0*individual.thickness,(10.0/3.0)*individual.radius))
                    axisZ=max(individual.thickness+1.0, 1.5*(individual.height-1.0)/float((2*(number_of_cavities+1))))
                else:
                    axisX=max(individual.height/2.5 , max(individual.thickness+1.0, (7.0/3.0)*individual.radius))
                    axisY=max(individual.height/2.5 , max(individual.thickness+1.0, (7.0/3.0)*individual.radius))
                    axisZ=max(individual.thickness+1.0, 1.5*(individual.height-1.0)/float((2*(number_of_cavities+1))))

            cavity=[height,aType,axisX,axisY,axisZ]
            accordion.addCavity(individual,cavity)

        cavityBottom=[0.5,"cylinder",5.0*individual.radius/3.0,5.0*individual.radius/3.0,0.75]
        cavityTop=[individual.height,"cylinder",5.0*individual.radius/3.0,5.0*individual.radius/3.0,0.75]

        accordion.addCavity(individual,cavityBottom)
        accordion.addCavity(individual,cavityTop)

        individual.level=None

        return individual


def generateFunc(numgen, params):
    nbInd = params["nbInd"]
    pop = algorithm.Population()

    for i in range(nbInd):
        ltype=random.choice(type)
        pop.append(generateIndividual(ltype))

    return pop

####################################################################################################
### Eval
###
def evaluationFunc(pop):
    global thickness
    print("Evaluation Function "+str(len(pop)))
    basedir=os.path.dirname(__file__)
    bestscore = -float(sys.maxint)

    filename=[]
    for ind in pop:
        shape, shapeMinus = getShapeFromInd(ind)

        ### return (shape,shapeMinus)
        ###
        fend =  "def evalField(x,y,z): \n\treturn shape.eval(primitives.Point(x,y,z))"
        f1 = shapewriter.toPythonString(shape) + fend
        f2 = shapewriter.toPythonString(shapeMinus) + fend

        filename.append((f1,f2, ind))

    #################### EXAMPLE USING THE SEQUENTIAL LAUNCHER #################################
    ### List of filename that contains the simulation to run
    scenefiles = ["scene.pyscn","controller.py", "shape.py", "shapeinv.py"]
    filesandtemplates = []
    for scenefile in scenefiles:
        filesandtemplates.append( (open(basedir+"/"+scenefile).read(), scenefile) )

    for f1, f2, ind in filename:
        runs = []
        for f1,f2,ind in filename:
            runs.append( {"GENERATION": str(pop.id),
                          "INDIVIDUAL": str(ind.id),
                          "SHAPECONTENT": f1, "SHAPEINVCONTENT": f2, "nbIterations":10,
                          "LIBRARYPATH" : os.path.dirname(geneticalgo.__file__),
                          "THICKNESS" : thickness
                          } )

    results = launcher.startSofa(runs, filesandtemplates, launcher=launcher.SerialLauncher())

    for res in results:

               print("Results: ")
               print("    directory: "+res["directory"])
               print("        scene: "+res["scene"])
               print("      logfile: "+res["logfile"])
               print("     duration: "+str(res["duration"])+" sec")

    ### Associate the results to the individuals.
    for i in range(len(filename)):
        f1, f2, ind = filename[i]
        ind.results = results[i]
        data = getJSONFragmentFrom( ind.results["logfile"] )


        if data == None:
            print "SOFA CRASHED DOWN!!  TRY TO LAUNCH MANUALLY scene.pyscn"
            ind.level = - float(sys.maxint)
        else:

            Z0 = data["Z0"]
            Zmax = data["Zmax"]
            V0 = data["V0"]

            level =(Zmax-Z0)


            if level > 20.0:
                print "a scene is ill defined, excessive result"
                ind.level = - float(sys.maxint)
            elif level < 0.0:
                print "becareful, negative result"
                ind.level = level
            else:
                ind.level = level



    ## Wait here the results.
    for f1,f2,ind in filename:
        if ind.level > bestscore:
            bestscore = ind.level

    return bestscore


####################################################################################################
### Select
###
def selectionFunc(pop, params):

    nbInd=params["nbInd"]

    if not isinstance(nbInd, int):
        raise ValueError, "nbInd is not given or not an int"
    pop.sortAccordingToLevel()
    pop.deleteFrom(params["nbInd"])
#    pop.sortAccordingToIndex()

    return pop
