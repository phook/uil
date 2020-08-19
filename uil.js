let global = this;
let uilParser = null;

function load(url,fn)
{
    var oReq = new XMLHttpRequest();
    oReq.addEventListener('load', function() {fn(this.responseText)})
    oReq.open('GET', url);
    oReq.send();
}

load("https://raw.githubusercontent.com/phook/BNFT/master/BNFT.js", function(source) {
        eval.call(global, source)

        load("./uil.BNFT", function(source) {
                uilParser = new global.BNFT(source, {
                    alert: console.log
                }); 
            });

    });



function getMaxX()
{
    let add = this.classList.contains("row");
    let children = this.getElementsByTagName("div");
    let maxX = this.style.maxHeight ? this.style.maxHeight : add ? 9e9 : 0;
    if (children.length)
    {
        for(let i in children)
        if (children[i].parentElement === this)
        {
            let childMaxX = children[i].getMaxX();
            if (add)
                maxX += childMaxX;
            else
                maxX = maxX > childMaxX ? maxX : childMaxX;
        }
    }
    else
    {
        if (this.style.flexGrow)
            maxX = 9e9;
        else
            maxX=this.clientWidth; 
    }
    return maxX;
}
function getMaxY()
{
    let add = this.classList.contains("col");
    let children = this.getElementsByTagName("div");    
    let maxY = this.style.maxHeight ? this.style.maxHeight : add ? 9e9 : 0;
    if (children.length)
    {
        for(let i in children)
        if (children[i].parentElement === this)
        {
            let childMaxY = children[i].getMaxY();
            if (add)
                maxY += childMaxY;
            else
                maxY = maxY > childMaxY ? maxY : childMaxY;
        }
    }
    else
    {
        if (this.style.flexGrow)
            maxY = 9e9;
        else
            maxY = this.clientHeight; 
    }
    return maxY;
}
function getMinX()
{
    let add = this.classList.contains("row");
    let children = this.getElementsByTagName("div");
    let minX = this.style.minWidth ? this.style.minWidth : add ? 9e9 : 0;
    if (children.length)
    {
        for(let i in children)
        if (children[i].parentElement === this)
        {
            let childMinX = children[i].getMinX();
            if (add)
                minX += childMinX;
            else
                minX = minX > childMinX ? minX : childMinX;
        }
    }
    else
    {
        if (this.style.flexGrow)
            minX = 0;
        else
            minX=this.clientWidth; 
    }
    return minX;
}
function getMinY()
{
    let add = this.classList.contains("col");
    let children = this.getElementsByTagName("div");    
    let minY = this.style.minHeight ? this.style.minHeight : add ? 9e9 : 0;
    if (children.length)
    {
        for(let i in children)
        if (children[i].parentElement === this)
        {
            let childMinY = children[i].getMinY();
            if (add)
                minY += childMinY;
            else
                minY = minY > childMinY ? minY : childMinY;
        }
    }
    else
    {
        if (this.style.flexGrow)
            minY = 0;
        else
            minY = this.clientHeight; 
    }
    return minY;
}

function traverse()
{
    let col = this.classList.contains("col");
    let row = this.classList.contains("row");
    let children = this.getElementsByTagName("div");    
    if (children.length)
    {
        for(let i in children)
        if (children[i].parentElement === this)
        {
            children[i].traverse();
        }
    }
    if  (row)
        if (this.getMaxY() > this.getMinY())
            this.style.flexGrow="1";
    if  (col)
        if (this.getMaxX() > this.getMinX())
            this.style.flexGrow="1";
}


function doTraverse()
{
    let alldiv = document.getElementsByTagName("div");
    for(let i in alldiv)
    {
        alldiv[i].getMaxX = getMaxX;
        alldiv[i].getMaxY = getMaxY;
        alldiv[i].getMinX = getMinX;
        alldiv[i].getMinY = getMinY;
        alldiv[i].traverse = traverse;
    }
    let list = document.getElementsByClassName("body");
    list[0].traverse();
}
