#!/usr/bin/env python
import web
import os

from time import strftime
from time import gmtime
from subprocess import check_output
from subprocess import call

urls = (
    '/hRecorder', 'Recorder',
    '/sofa', 'Upload',
    '/video', 'Download'
)

app = web.application(urls, globals())
textAreaContent = ""
filedir = '/home/douaille/tmp'

class Recorder:
    def GET(self):
        sceneFile = filedir +'/'+ currentFileName + ".scn"
	stdout = check_output(["//home/douaille/SOFA/build-HeadlessRecorder-Desktop_Qt_5_9_2_GCC_64bit-Default/bin/runSofa", sceneFile, "-g", "hRecorder", "--video", "--fps=60", "-a", "--filename", currentFileName])
        html = """<html><head></head><body>
</body></html><textarea readonly rows="10" cols="100">"""
        html += stdout
        html += """</textarea><br/><br/><a href="video?file="""
        html += currentFileName
        html += """.avi" class="button">Download video</a></body></html>"""
	return html

class Upload:
    def GET(self):
        return """

        <html>

        <head>
        <meta charset="UTF-8">
        <link rel=stylesheet href="//codemirror.net/lib/codemirror.css">
        <script src=//codemirror.net/lib/codemirror.js></script>
        <script src=//codemirror.net/mode/xml/xml.js></script>
        <script src="//cdn.rawgit.com/vkiryukhin/vkBeautify/master/vkbeautify.js"></script>
        </head>

        <body>
        <form method="POST" enctype="multipart/form-data" action="">
        Write your scene :
        <textarea id="test" name="scene"><?xml version="1.0" encoding="UTF-8" ?><note><to>Tove</to><from>Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note></textarea>
        <script>
        document.getElementById("test").value = vkbeautify.xml(document.getElementById("test").value);
        CodeMirror.fromTextArea(document.getElementById("test"), {
        mode: 'application/xml',
        lineNumbers: true,
        lineWrapping: true,
        });
        </script>
        <br/>
        <br/>
        Or choose a file :
        <input type="file" name="myfile" />
        <br/>
        <br/>
        <input type="submit">
        </form>
        </body></html>"""

    def POST(self):
        global currentFileName
        currentFileName = strftime("tmp-%Y-%m-%d_%H-%M-%S", gmtime())
        scene = web.input().scene
        files = web.input(myfile={})
        if not scene:
            if 'myfile' in files: # to check if the file-object is created
                if files.myfile.filename == "":
                    raise  web.notfound()
                extension = os.path.splitext(files.myfile.filename)[1]
                print extension
                if extension != ".scn":
                    raise  web.notfound() #should raise an other error ...
                filename = currentFileName + ".scn"
                fout = open(filedir +'/'+ filename,'w') # creates the file where the uploaded file should be stored
                fout.write(files.myfile.file.read()) # writes the uploaded file to the newly created file.
                fout.close()
            else:
                raise web.notfound( )
        else:
            filename = currentFileName + ".scn"
            fout = open(filedir +'/'+ filename,'w') # creates the file where the uploaded file should be stored
            fout.write(scene) # writes the uploaded file to the newly created file.
            fout.close()
        raise web.seeother('/hRecorder')

class Download:
    def GET(self):
        request = web.input().file
        getPath = "./"
        getPath += request
        print "---------------------------------------------------------- Download"
        print getPath
        print "----------------------------------------------------------"
        if os.path.exists( getPath ):
            web.header("Content-Disposition", "attachment; filename=%s" % request)
            web.header("Content-Type", "video/x-msvideo")
            web.header("Transfer-Encoding","chunked")
            f = open(os.path.join("./", request), "rb")
            while 1:
                buf = f.read(1024 * 8)
                if not buf:
                    break
                yield buf
        else:
            raise web.notfound( )

if __name__ == "__main__":
    app.run()
