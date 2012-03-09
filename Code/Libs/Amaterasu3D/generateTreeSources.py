import os
import sys
import glob

IGNORE = ['.\\glm','.\\Addons\\Binvox']

class SourceDir:
	def __init__(self):
		self.sources = []
		self.headers = []
		self.dir = ""
		
	def updateFiles(self, dir):
		self.dir = dir.replace("\\","/")[2:]
		#print "Dir",self.dir
		self.sources = glob.glob(dir+'/*.cpp')
		self.headers = glob.glob(dir+'/*.hpp')
		self.headers.extend(glob.glob(dir+'/*.h'))
		self.headers.extend(glob.glob(dir+'/*.inl'))
	
		self.headers = [s.replace("\\","/")[2:] for s in self.headers]
		self.sources = [s.replace("\\","/")[2:] for s in self.sources]
	
		#print "Sources:", self.sources
		#print "Headers:", self.headers
	
	def __str__(self):
		if((len(self.sources) + len(self.headers)) == 0):
			return ""
		s = "# Dir name " +  self.dir + "\n"
		if(len(self.sources) != 0):
			s += "SET(SOURCES_AMA3D ${SOURCES_AMA3D} \n"
			for src in self.sources:
				s += "\t"+src+"\n"
			s+=")\n"
			s+="\n"
		if(len(self.headers) != 0):
			s+= "SET(HEADERS_AMA3D ${HEADERS_AMA3D} \n"
			for head in self.headers:
				s += "\t"+head+"\n"
			s+=")\n"
			s+="\n"
		return s
		

def callbackWalk(arg, directory, files):
	print "#Walk : " + directory
	for i in IGNORE:
		if(directory.find(i) != -1):
			print "#Ignore : " + directory
			return
	s = SourceDir()
	s.updateFiles(directory)
	#print str(s)
	arg.append(s)
		
if __name__=="__main__":
	srcDirs = []
	os.path.walk(".", callbackWalk, srcDirs)
	print "==========="
	for s in srcDirs:
		print str(s)