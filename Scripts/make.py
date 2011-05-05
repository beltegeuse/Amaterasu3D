import sys
import os
import subprocess
import time

headers = [] # 

def Usage():
    print("make.py Version 0.2 pour EodeGame")
    print("Usage : ")
    print("")
    print("make.py init [release|debug] : Pour initialiser l'enviroment de developpement.")
    print("make.py make [flags] : Pour compiler le projet avec les flags qui vont bien.")

class Compilation:
    def __init__(self):
        self.__platform = sys.platform
        self.__path = "Build/"
        self.InitialiseRootDirectory()

    # Pour bien se positionner
    def InitialiseRootDirectory(self):
        print("[INFO] Positionnement dans le repertoire.")
        path = os.path.abspath(os.path.curdir)
        path = path.split(os.path.sep)
        if(path[-1] == "scripts"):
            print("[INFO] Cd ..")
            os.chdir("..")

    def Initialisation(self):
        self.Initialisation("debug")

    # Initilisation de l'environement.
    def Initialisation(self, type):
        print("[INFO] Initialisation ...")
        
        # Pour verifier que les flags soient Bon
        if(type != "debug" and type != "release"):
            print("[ERREUR] Flags : " + type)
            print("[INFO] soit release/debug")
            return

        if(not os.path.exists("Build")): 
            print("[INFO] Mkdir Build.")
            os.mkdir("Build")

        # Construction de la Commande
        commande = 'cmake -E chdir Build/ cmake -DCMAKE_BUILD_TYPE=' + type + ' '

        # Les instructions specifique a la platforme
        if(self.__platform == 'win32'):
            commande += '-G "MinGW Makefiles"'
        elif(self.__platform == 'linux2'):
            commande += '-G "Unix Makefiles"'
        else:
            print("[ERROR] UNsupported Platform : " + self.__platform)
            return
        
        #Pour l'install
        commande += " -DCMAKE_INSTALL_PREFIX=Build/"

        # On ajoute la fin de la commande
        commande += ' ..'
        
        


        os.system(commande)

    def CompilerAll(self, args):
        self.Compiler(args)
        
        for el in headers:
            print("")
            print("======================== ")
            print("======== " + el + " ========= ")
            print("======================== ")
            print("")
            sys.stdout.flush()
            self.Compiler([el])

    def Compiler(self,args):
        #print(" Args = " + str(args))
        commande = ""
        
        # On ajoute les options specifique a la platforme
        if(self.__platform == 'win32'):
            #print("[INFO] Windows Platform.")
            commande += "mingw32-make"
        elif(self.__platform == 'linux2'):
            #print("[INFO] Linux Platform.")
            commande += "make"
        else:
            print("[ERROR] Unsupported Platform : " + self.__platform)
            return

        # Ajout des options normales
        commande += " -f Makefile"
        
        #On ajoutes les flags si il y a ...
        for arg in args:
            commande += " " + arg

        self.__searchBuildDirAndExecute(commande)
        

    def __searchBuildDirAndExecute(self, commande):
        if(os.access("Build", os.R_OK)):
            os.chdir("Build")
            os.system(commande)
        elif(os.access("..//Build", os.R_OK)):
            os.chdir("..//Build")
            os.system(commande)
        else:
            print("[ERROR] No Build Directory")

if __name__=="__main__":
    print("Compilation ...")
    c = Compilation()
    if(len(sys.argv) > 1):

        # On va choisir la commande
        instruction = sys.argv[1]
        print("[INFO] Instruction : " + instruction)
        if(instruction == "init"):
            if(len(sys.argv) == 3):
                c.Initialisation(sys.argv[2])
            else:
                c.Initialisation()
        else: # On va demander la compilation
            c.CompilerAll(sys.argv[1:])
    else:
        c.CompilerAll([])
