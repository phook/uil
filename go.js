let BNFT=require("../BNFT/BNFT.js");
let fs=require("fs");
let exec=require("child_process").exec;
let fileToString = function(filename)
{
    return fs.readFileSync(filename, {option:'utf8', function(err, source) {console.log("error reading "+filename);throw err;}}).toString();
}


let target=

{
    "result" : "",
    "_in_begin" : function()
    {
       this.result = "<html><head><link rel='stylesheet' type='text/css' href='uil.css'/><script src='uil.js'></script></head><body onLoad='doTraverse()'>" 
    },
    "_in_end" : function()
    {
        this.result += "</body></html>\";"
    },
    "_in_class_begin" : function(name, parameters)
    {
        // HERE THE CLASS NAME NEEDS TO BE CHECKED AGAINST TEMPLATES
        this.result += "<div class='" + name + "' style='";
        for(key in parameters)
            this.result += key + ":" + parameters[key] + ";";
        this.result +=  "'>";
    },
    "_in_class_end" : function()
    {
        this.result += "</div>";
    },
    "_in_text" : function(text)
    {
        this.result += text;
    },
    "save" : function(filename)
    {
        fs.writeFileSync(filename, this.result);
    }
}




let uil_syntax = fileToString("uil_to_html.bnft");
let parser = new BNFT(uil_syntax,{alert:console.log});
if (parser)
{
    let source = fileToString("test.uil");
    console.log(source);
    let result = parser.parse(source,{alert:console.log});
    console.log(result);
    fs.writeFileSync("result.html", result);


    //result = eval(result);



//    target.save("result.html",result);
    exec("result.html");
}
