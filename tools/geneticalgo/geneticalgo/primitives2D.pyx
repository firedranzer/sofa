
cdef int i=0

cpdef str generateNewSubIndex():
    global i
    cdef str index=str(i)
    i+=1
    return index

cpdef void clearOut():
    global i
    i=0

cdef class ListOfLitteralExpressions(object):


    def __init__(self):

        self.listA=[]
        self.listB=[]

        self.listgradientXTemp=[]
        self.listgradientYTemp=[]
        self.listSqrt=[]


    cpdef ListOfLitteralExpressions plus(self, ListOfLitteralExpressions listOfLitteralExpressions):

        newList=ListOfLitteralExpressions()
        newList.listA=self.listA + listOfLitteralExpressions.listA
        newList.listB=self.listB + listOfLitteralExpressions.listB

        newList.listgradientXTemp=self.listgradientXTemp + listOfLitteralExpressions.listgradientXTemp
        newList.listgradientYTemp=self.listgradientYTemp + listOfLitteralExpressions.listgradientYTemp
        newList.listgradientZTemp=self.listgradientZTemp + listOfLitteralExpressions.listgradientZTemp
        newList.listSqrt=self.listSqrt + listOfLitteralExpressions.listSqrt

        return newList


cdef class ListOfPrimitives(object):

    def __init__(self):

        self.listPrimitives=[]

    cpdef ListOfPrimitives plus(self, ListOfPrimitives listOfPrimitives):

        newList=ListOfPrimitives()
        newList.listPrimitives=self.listPrimitives + listOfPrimitives.listPrimitives
        return newList

cdef class ListForWriting(object):

    def __init__(self):

        self.listWritingA=[]
        self.listWritingB=[]

    cpdef ListForWriting plus(self, ListForWriting listForWriting):

            newList=ListForWriting()
            newList.listWritingA=self.listWritingA + listForWriting.listWritingA
            newList.listWritingB=self.listWritingB + listForWriting.listWritingB

            return newList

cdef class Point2D(object):

    def __init__(self, x, y):
        self.x=x
        self.y=y

    cdef display(self):
        print "("+str(self.x),str(self.y)+")"
        return


cdef Point2D translationRotation(double sinTheta, double cosTheta, Point2D center, Point2D point):

    cdef double x
    cdef double y

    x=cosTheta*(point.x-center.x)-sinTheta*(point.y-center.y)
    y=sinTheta*(point.x-center.x)+cosTheta*(point.y-center.y)

    point=Point2D(x,y)

    return point


cdef class Shape2D(object):

    def __init__(self, str index):

        self.index=index
        self.subIndex=generateNewSubIndex()

    cpdef double eval(self, Point2D point):
        return self.eval(point)

    cpdef tuple toString(self):
        return self.toString()

    cpdef str toWriting(self):
        return self.toWriting()

    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.getListOfPrimitives()

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):
        return self.getListOfLitteralExpressions()


    cpdef ListForWriting getListForWriting(self):
        return self.getListForWriting()


cdef class Union(Shape2D):

    def __init__(self, Shape2D first, Shape2D second, str index):
        Shape.__init__(self, index)
        self.first=first
        self.second=second

    cpdef double eval(self,Point2D point):
        return min(self.first.eval(point),self.second.eval(point))


    cpdef tuple toString(self):

        expression="min(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")"

        gradX="(1.0-ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+"))*gradA"+self.index+"_"+self.subIndex+"X"+"+"+"ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")*gradB"+self.index+"_"+self.subIndex+"X"
        gradY="(1.0-ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+"))*gradA"+self.index+"_"+self.subIndex+"Y"+"+"+"ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")*gradB"+self.index+"_"+self.subIndex+"Y"

        return (expression,(gradX,gradY))


    cpdef str toWriting(self):

        writing="primitives2D.Union(WritingA"+self.index+"_"+self.subIndex+",WritingB"+self.index+"_"+self.subIndex+")"

        return writing


    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y))=self.first.toString()
        (expression2,(grad2X,grad2Y))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"_"+self.subIndex+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"_"+self.subIndex+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"_"+self.subIndex+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"_"+self.subIndex+"Y="+grad1Y)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"_"+self.subIndex+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"_"+self.subIndex+"Y="+grad2Y)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"_"+self.subIndex+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"_"+self.subIndex+"="+writing2)
        return listForWriting



cdef class Intersection(Shape2D):

    def __init__(self, Shape2D first, Shape2D second, str index):
        Shape.__init__(self, index)
        self.first=first
        self.second=second

    cpdef double eval(self,Point2D point):
        return max(self.first.eval(point),self.second.eval(point))

    cpdef tuple toString(self):

        expression="max(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")"

        gradX="ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")*gradA"+self.index+"_"+self.subIndex+"X"+"+"+"(1.0-ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+"))*gradB"+self.index+"_"+self.subIndex+"X"
        gradY="ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")*gradA"+self.index+"_"+self.subIndex+"Y"+"+"+"(1.0-ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+"))*gradB"+self.index+"_"+self.subIndex+"Y"

        return (expression,(gradX,gradY))


    cpdef str toWriting(self):

        writing="primitives2D.Intersection(WritingA"+self.index+"_"+self.subIndex+",WritingB"+self.index+"_"+self.subIndex+")"

        return writing

    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y))=self.first.toString()
        (expression2,(grad2X,grad2Y))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"_"+self.subIndex+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"_"+self.subIndex+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"_"+self.subIndex+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"_"+self.subIndex+"Y="+grad1Y)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"_"+self.subIndex+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"_"+self.subIndex+"Y="+grad2Y)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"_"+self.subIndex+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"_"+self.subIndex+"="+writing2)
        return listForWriting


cdef class Difference(Shape2D):

    def __init__(self, first, second):
        Shape.__init__(self)
        self.first=first
        self.second=second

    cpdef double eval(self,Point2D point):
        return max(self.first.eval(point),-self.second.eval(point))


    cpdef tuple toString(self):

        expression="max(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+")"

        gradX="ind(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+")*gradA"+self.index+"_"+self.subIndex+"X"+"+"+"(1.0-ind(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+"))*(-gradB"+self.index+"_"+self.subIndex+"X"+")"
        gradY="ind(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+")*gradA"+self.index+"_"+self.subIndex+"Y"+"+"+"(1.0-ind(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+"))*(-gradB"+self.index+"_"+self.subIndex+"Y"+")"

        return (expression,(gradX,gradY,gradZ))


    cpdef str toWriting(self):

        writing="primitives.Difference(WritingA"+self.index+"_"+self.subIndex+",WritingB"+self.index+"_"+self.subIndex+")"

        return writing


    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y,grad1Z))=self.first.toString()
        (expression2,(grad2X,grad2Y,grad2Z))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"_"+self.subIndex+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"_"+self.subIndex+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"_"+self.subIndex+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"_"+self.subIndex+"Y="+grad1Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradA"+self.index+"_"+self.subIndex+"Z="+grad1Z)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"_"+self.subIndex+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"_"+self.subIndex+"Y="+grad2Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradB"+self.index+"_"+self.subIndex+"Z="+grad2Z)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"_"+self.subIndex+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"_"+self.subIndex+"="+writing2)

        return listForWriting



cdef class Primitives2D(Shape2D):

    def __init__(self, str index):
        Sgape2D.__init__(self, index)
        self.type
        self.identifier

    cpdef double eval (self, Point2D point):
        return self.eval(point)

    cpdef ListOfPrimitives getListOfPrimitives(self):

        cdef ListOfPrimitives listOfPrimitives=ListOfPrimitives()

        listOfPrimitives.listPrimitives.append(self.identifier)

        return listOfPrimitives



    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=ListOfLitteralExpressions()

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=ListForWriting()

        return listForWriting


cdef class Ellipse(Primitives2D):

    def __init__(self,axisX,axisY):

        Primitives2D.__init__(self)
        self.type="ellipse"
        self.axisX= axisX
        self.axisY= axisY
        identifier=[self.type, self.axisX,self.axisY,self.index, self.subIndex]
        self.identifier=identifier

    cpdef double eval(self,Point2D point):

        return (point.x/self.axisX)**2+(point.y/self.axisY)**2-1.0

    cpdef tuple toString(self):

        (x,y)=("x"+self.index,"y"+self.index)
        (dxX,dxY)=("dxX"+self.index,"dxY"+self.index)

        gradX="("+dxX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dxY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"


        (dyX,dyY)=("dyX"+self.index,"dyY"+self.index)

        gradY="("+dyX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dyY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"

        (dzX,dzY)=("dzX"+self.index,"dzY"+self.index)

        gradZ="("+dzX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dzY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"

        grad=(gradX,gradY,gradZ)

        expression="("+x+"/axis"+self.index+"X)*("+x+"/axis"+self.index+"X)"+"+"+"("+y+"/axis"+self.index+"Y)*("+y+"/axis"+self.index+"Y)-1.0"

        return (expression, grad)

    cpdef str toWriting(self):

        return "Ellipse"+self.index+"_"+self.subIndex

cdef class HalfPlaneGivenByAnOrientedCoupleOfPoints(Primitives2D):

    """
    The half_plane will be on the right of the oriented couple
    """
    def __init__(self,Point2D first, Point2D second, index):

        Primitives2D.__init__(self, index)
        self.type="half-plane"
        self.first=first
        self.second=second
        self.identifier=[self.type, self.first,self.second, self.index, self.subIndex]


    cpdef double eval(self,Point2D point):

        cdef Point2D center=self.first

        cdef double l=math.sqrt((self.second[0]-self.first[0])*(self.second[0]-self.first[0])\
                               +(self.second[1]-self.first[1])*(self.second[1]-self.first[1]))

        cdef double sinTheta=(self.second[1]-self.first[1])/l
        cdef double cosTheta=(self.second[0]-self.first[0])/l

        point=translationRotation(sinTheta, cosTheta, center, point)

        return point.y

    cpdef tuple toString(self):

        (x,y)=("x"+self.index,"y"+self.index)

        gradX="dxY"+self.index
        gradY="dyY"+self.index
        gradZ="dzY"+self.index
        grad=(gradX,gradY,gradZ)

        expression=y

        return (expression, grad)

    cpdef str toWriting(self):
        return "HalfPlane"+self.index+"_"+self.subIndex

cpdef list createPolygoalChain(list listOfCouples):

    newList=[]

    for couple in listOfCouplesOfPoints:
        point=Point2D(couple[0],couple[1])

        newList.append(point)

    return newList

cpdef Shape2D closedPolygonalChain(list listOfCouple, str index):

    """
    Becareful! The last and the first point will be connected! The body of the shape will be on the right of the line, according to its
    orientation. It may not be bounded!!
    """
    cdef list listOfPoints = createPolygoalChain(listOfCouples)
    length=len(listOfPoints)

    if length<=2:

        raise ValueError, "I need more than two points!!"

    newShape=HalfPlaneGivenByAnOrientedCoupleOfPoints(listOfPoints[0], listOfPoints[1], index)

    for i in range(1,length-1):

        newShape=Intersection(newShape, HalfPlaneGivenByAnOrientedCoupleOfPoints(listOfPoints[i], listOfPoints[i+1], index))

    newShape=Intersection(newShape, HalfPlaneGivenByAnOrientedCoupleOfPoints(listOfPoints[length-1], listOfPoints[0], index))


    return newShape
