const ELEMENT_NODE=1;
let templates={};

function getMaxX()
{
    let col = this.classList.contains("col");
    let maxX = 9e9;
    if (this.uilChildren.length)
    {
        for(let i in this.uilChildren)
        if (this.uilChildren[i].parentElement === this)
        {
            let childMaxX = getMaxX.call(this.uilChildren[i]);
            if (col)
                maxX += childMaxX;
            else
                maxX = maxX < childMaxX ? maxX : childMaxX;
        }
    }
    else
    {
        maxX =  9e9;
    }
    if (this.style.maxWidth && (maxX > toNumber(this.style.maxWidth)))
        maxX = toNumber(this.style.maxWidth);
    return toNumber(maxX);
}
function getMaxY()
{
    let row = this.classList.contains("row");
    let maxY = 9e9;
    if (this.uilChildren.length)
    {
        for(let i in this.uilChildren)
        if (this.uilChildren[i].parentElement === this)
        {
            let childMaxY = getMaxY.call(this.uilChildren[i]);
            if (row)
                maxY += childMaxY;
            else
                maxY = maxY < childMaxY ? maxY : childMaxY;
        }
    }
    else
    {
        maxY = 9e9; 
    }
    if (this.style.maxHeight && (maxY > toNumber(this.style.maxHeight)))
        maxY = toNumber(this.style.maxHeight);
    return toNumber(maxY);
}
function getMinX()
{
    let col = this.classList.contains("col");
    let minX = 0;
    if (this.uilChildren.length)
    {
        for(let i in this.uilChildren)
        if (this.uilChildren[i].parentElement === this)
        {
            let childMinX = getMinX.call(this.uilChildren[i]);
            if (col)
                minX += childMinX;
            else
                minX = minX > childMinX ? minX : childMinX;
        }
    }
    if (this.style.minWidth && (minX < toNumber(this.style.minWidth)))
        minX = toNumber(this.style.minWidth);
    return toNumber(minX);
}
function getMinY()
{
    let row = this.classList.contains("row");
    let minY = 0;
    if (this.uilChildren.length)
    {
        for(let i in this.uilChildren)
        if (this.uilChildren[i].parentElement === this)
        {
            let childMinY = getMinY.call(this.uilChildren[i]);
            if (row)
                minY += childMinY;
            else
                minY = minY > childMinY ? minY : childMinY;
        }
    }
    if (this.style.minHeight && (minY < toNumber(this.style.minHeight)))
        minY = toNumber(this.style.minHeight);
    return toNumber(minY);
}

function toNumber(n,d)
{
    if (d === 1 && n === "")
        return 9e9;
    if (d === -1 && n === "")
        return 0;
    return Number((n+"").replace("px",""));
}

function toPixels(n)
{
        return n+"px";
}
function getStylePropertySetListener(target, attribute, arg, value, expression)
{
    return function(value)
    {
        // possible error for subset argument names - requires more advanced replacement
        let newValue = expression.replaceAll(":"+arg,value);
        target.style.setProperty(attribute, newValue); 
    }
}

function getAttributeSetListener(target, attribute, arg, value, expression)
{
    return function(value)
    {
        // possible error for subset argument names - requires more advanced replacement
        let newValue = expression.replaceAll(":"+arg,value);
        target.setAttribute(attribute, newValue); 
    }
}

function getTextNodeSetListener(target, arg, value, expression)
{
    return function(value)
    {
        // possible error for subset argument names - requires more advanced replacement
        let newValue = expression.replaceAll(":"+arg,value);
        target.data = newValue; 
    }
}

const styleProxyHandler = 
{
    get(target, property)
    {
        if (target._uilStyle[property])
            return target._uilStyle[property];
        return undefined;
    },
    set(target, property, value) 
    {
        if (Object.keys(target.style).includes(property))
            target.style[property] = value;
        if (target._uilStyle[property])
        {
            target._uilStyle[property] = value;
            if (target._uilStyleListeners)
            {
                if (Object.keys(target._uilStyleListeners).includes(property))
                {
                    target._uilStyleListeners[property].forEach(function(listener)
                    {
                        listener(value);
                    });
                }
            }
        }
    },
    ownKeys: function(target) 
    {
        return Object.keys(target);
    },
    getOwnPropertyDescriptor: function(target, key) 
    {
        return { enumerable: true, configurable: true };
    }
}

let attributes_to_transfer=
[
    "min-width",
    "max-width",
    "background-color",
    "font-family",
    "color",
    "font-size",
    "align-items",
    "flex",
    "flex-grow",
    "flex-shrink",
    "flex-basis",
    "height",
    "min-height",
    "max-height",
    "width",
    "min-width",
    "max-width",
    "display",
    "overflow",
    "overflow-x",
    "overflow-y",
    "pointer-events",
    "padding-left",
    "padding-right",
    "padding-top",
    "padding-bottom",
    "padding"
];

function toJavascript(arg)
{
}

function transferUilStyle(skipArguments)
{
    //if (!skipArguments)
    {
        keys = Object.keys(this._uilStyle);
        for(key in keys)
        {
            let type=typeof(this._uilStyle[keys[key]]);
            if (type !== "function" && type !== "object")
            {
                // calculate 
                if (/*[...this.style].includes(keys[key]) ||*/ attributes_to_transfer.includes(keys[key]))
                {
                    let value = this._uilStyle[keys[key]];
                    let match = value.match(/.*(\:|\+|\-|\*|\/).*/);
                    if (match !== null)
                    {
                        value=value.replace(/:/g,"this._uilStyle['_template_parent']._uilStyle.");
                        value = eval(value);
                    }
                    this.style.setProperty(keys[key],value);
//                    this.style[keys[key]] = value; 
                }
            }
        }
    }

}


// CHANGE NAME TO uilInitialize()
function doTraverse()
{
    uilInitialize()
}

/// BELOW NEW
function uilInitialize()
{
    // collect template and remove from DOM
    templatesElements = [...document.getElementsByClassName("template")];
    templatesElements.forEach(template => {templates[template.id] = template; template.remove()});

    // build uilTree and instance templates
    let uilBody = [].slice.call(document.getElementsByClassName("body"))[0]; 

        var t0 = performance.now()
    DOMTraverse.call(uilBody);
        var t1 = performance.now()
        console.log("Dom Traverse took " + (t1 - t0) + " milliseconds.")
    uilTraverse.call(uilBody);
        var t2 = performance.now()
        console.log("UIL Traverse took " + (t2 - t1) + " milliseconds.")
}

function DOMTraverse(parent)
{
    var current = this;
    // uilElements can only be ELEMENT_NODE
    if (current.nodeType === ELEMENT_NODE)
    {
        // handle templates
        uilAttribute = current.getAttribute("uilElement");
        if (uilAttribute)
            current.uilName = current.className;

        if (!current.templateLoaded && current.className in templates)
        {
            let uilArguments = current.getAttribute("data-uilstyle"); // should be renamed to arguments
            uilArguments = JSON.parse(uilArguments);
            if (!uilArguments)
                uilArguments = {};
            template = templateClone(current.className, [...current.childNodes], uilArguments);
            current.parentElement.insertBefore(template,current);
            current.remove();
            current = template;
            current.setAttribute("uilElement","true");
            rerunScripts.call(current);
        }
        else
        {
            if (uilAttribute && current.classList[0] !== "script")
                current.style.setProperty("flex","1 1 auto");
        }
        
        if (current.style.height != "")
        {
            current.style.minHeight = current.style.height;
            current.style.maxHeight = current.style.height;
        }
        if (current.style.width != "")
        {
            current.style.minWidth = current.style.width;
            current.style.maxWidth = current.style.widtk;
        }
        
        // build uilTree
        if (uilAttribute)
        {
            current.uilChildren=[];

            if (!current._uilStyle)
            {
                let uilStyle = current.getAttribute("data-uilstyle");
                current._uilStyle = uilStyle ? JSON.parse(uilStyle) : {};
            }
            
            if (!current.uilStyle)
            {
                current.uilStyle = new Proxy(current,styleProxyHandler); 
            }
            if (!current._uilStyleListeners)
                current._uilStyleListeners = {};

            if (parent)
            {
                current.uilParent = parent;
                parent.uilChildren.push(current);
            }
            parent = current;
        }
    }
    
    [...current.childNodes].forEach(child => DOMTraverse.call(child,parent));

}

// Sorts keys in "dawn" order - where matching starts of string results in the longer being on top
function dawnSort(a,b)
{ 
    if (b.indexOf(a) == 0)
    {
        return 1;
    }
    if (a.indexOf(b) == 0)
    {
        return -1;
    }
    return a>b?1:-1;
}

function DOMLookup(_identifier, from_owner)
{
    if (_identifier == this.id)
        return this;

    var originalIdentifier = _identifier;

    if (typeof(this.id) != "undefined")
    {
        if (this.id.charAt(-1) != "." && _identifier.indexOf(this.id) == 0)
        {
            if (_identifier.indexOf(this.id + ":") == 0)
                _identifier = _identifier.substring(this.id.length+1);
            else
                _identifier = _identifier.substring(this.id.length);
        }
    
    let result = DOMLookup_child.call(this, _identifier);
    
    if (result)
        return result;
    }
    
    if (this.parentNode && !from_owner)
        return DOMLookup.call(this.parentNode, originalIdentifier);
    
    return null;
}

function DOMLookup_child(_identifier)
{       
    for (let i = 0; i < this.children.length; i++) 
    {
        var result = DOMLookup.call(this.children[i],_identifier,true);
        if (result)
            return result;
    }
    return null;
}

function rerunScripts()
{
    let scripts = [].slice.call(this.getElementsByTagName("script"));
    for(let j=0; j<scripts.length; j++)
    {
        let scriptElement = document.createElement('script');
        scriptElement.type = "text/javascript";
        scriptSource = scripts[j].innerText.replace("/*","").replace(/(\*\/(?!.*\*\/))?/g,"");
        scriptElement.appendChild( document.createTextNode( scriptSource ) );
        scripts[j].parentElement.replaceChild(scriptElement,scripts[j]); // force rerun
    }
}

function findTemplateArguments(current,template,args)
{
    let keys = Object.keys(args);
//  if (keys.length === 0)
//       return;

    // DOM children attributes
    // Uil arguments
    // text elements
    
    [...current.childNodes].forEach(child => 
    {
        if (child.hasAttributes)
        {
            if (child.getAttribute("uilElement") !== true)
            {
                let attributes = child.attributes;
                for(let i = attributes.length - 1; i >= 0; i--) 
                {
                    for(let j = keys.length - 1; j >= 0; j--) 
                    {
                        if (attributes[i].value.match(new RegExp(".*:" + keys[j] + "\\b.*")) !== null)  
                        {
                            if (!template._uilStyleListeners[keys[j]])
                                template._uilStyleListeners[keys[j]] = []
                            template._uilStyleListeners[keys[j]].push(getAttributeSetListener(child, attributes[i].name, keys[j], args[keys[j]], attributes[i].value));
                        }
                    }
                }
                let value=child.getAttribute("uilStyle");
                if (value)
                {
                    for(let i=0; i<attributes_to_transfer.length;i++)
                    {
                        if (value.indexOf(attributes_to_transfer[i]+":") !== -1)
                        {
                            for(let j = keys.length - 1; j >= 0; j--) 
                            {
                                if (value.indexOf(":"+keys[j]) !== -1)
                                {
                                    if (!template._uilStyleListeners[keys[j]])
                                        template._uilStyleListeners[keys[j]] = []
                                    template._uilStyleListeners[keys[j]].push(getStylePropertySetListener(child, attributes_to_transfer[i], keys[j], args[keys[j]], ":"+keys[j]));
                                }
                            }
                        }
                    }
                }
            }
        }
        if (child.nodeType == 3/*Text*/)
        {
            for(let j = keys.length - 1; j >= 0; j--) 
            {
                if (child.data.indexOf(":"+keys[j]) !== -1)
                {
                    if (!template._uilStyleListeners[keys[j]])
                        template._uilStyleListeners[keys[j]] = [];
                    template._uilStyleListeners[keys[j]].push(getTextNodeSetListener(child, keys[j], args[keys[j]], child.data));
                }
            }
        }
        findTemplateArguments(child,template,args);
    });
}

function templateClone(templateName, contents, args)
{
    let template=null;
    if (templateName in templates)
    {
        template = templates[templateName].cloneNode(true);
        template.id = "";
        delete template.id;
        template.className = templateName;

        let templateChildren = [...template.childNodes];
                
        let contentMarker = template.getElementsByClassName(":content")[0];
        if (contentMarker)
        {
            if (typeof(contents) == "string")
            {
                contentMarker.parentElement.insertBefore(document.createTextNode(contents),contentMarker);
                //contentMarker.remove();
                contentMarker.style.setProperty("display","none");
            }
            else
            if (typeof(contents) == "object")
            {
                contents.forEach(element =>
                {
                    element.parentElement?.removeChild(element);
                    contentMarker.parentElement.insertBefore(element,contentMarker);
                });
                //contentMarker.remove();
                contentMarker.style.setProperty("display","none");
            }
        }


        /// ARGS SHOULD BE APPLIED HERE
        let uilArguments = template.getAttribute("data-uilstyle");
        uilArguments = JSON.parse(uilArguments);
        if (!uilArguments)
            uilArguments = {};
        template._uilStyle = {
                                ...uilArguments,
                                ...args,
                             };
        transferUilStyle.call(template);
        /// + uil/arg handlers to handle inDom changes

        template.setAttribute("uilElement",true);
        template.templateLoaded = true;
        DOMTraverse.call(template);
        uilTraverse.call(template);

        if (args)
        {
            template.uilStyle = new Proxy(template,styleProxyHandler); 
            template._uilStyleListeners = {};
            findTemplateArguments(template,template,template._uilStyle);

            let keys = Object.keys(template._uilStyle);
            for(let i=0; i<keys.length; i++)
            {
                template.uilStyle[keys[i]] = template._uilStyle[keys[i]];
            }
        }
    }
    return template;
}

function uilTraverse(parentRow, parentCol, parentLayers)
{
    let body = this.className === "body";
    let row = this.className === "row" || body; // body is also a row
    let col = this.className === "col";
    let layers = this.className === "layers";
    let filler = this.className === "filler";

    if (!(layers || filler || col || row))
        this.style.pointerEvents = "auto";
        
    
    this.uilChildren.forEach(child => uilTraverse.call(child, row, col, layers));

    // determine if container is fixed size or elastic
    if  (col || row || layers)
    {
        if (this.uilChildren.length)
        {
            this.style.maxHeight = toPixels(getMaxY.call(this));
            this.style.minHeight = toPixels(getMinY.call(this));
            this.style.maxWidth =  toPixels(getMaxX.call(this));
            this.style.minWidth =  toPixels(getMinX.call(this));
            if (row)
            {
                if ((toNumber(this.style.maxHeight,1) > toNumber(this.style.minHeight,-1)))
                    this.style.setProperty("flex","1 1 auto");
                else
                    this.style.setProperty("flex","0 0 auto");
            }
            if (col)
            {
                if ((toNumber(this.style.maxWidth,1) > toNumber(this.style.minWidth,-1)))
                    this.style.setProperty("flex","1 1 auto");
                else
                    this.style.setProperty("flex","0 0 auto");
            }
        }/*
        else
        {
            if (parentCol)
            {
                if (toNumber(this.style.maxHeight,1) > toNumber(this.style.minHeight,-1))
                    this.style.setProperty("flex","1 1 auto");
                else
                    this.style.setProperty("flex","0 0 auto");
            }
            if (parentRow)
            {
                if (toNumber(this.style.maxWidth,1) > toNumber(this.style.minWidth,-1))
                    this.style.setProperty("flex","1 1 auto");
                else
                    this.style.setProperty("flex","0 0 auto");
            }
        }*/
    }
    else
    {
        if (parentCol)
        {
            if (toNumber(this.style.maxHeight,1) > toNumber(this.style.minHeight,-1))
                this.style.setProperty("flex","1 1 auto");
            else
                this.style.setProperty("flex","0 0 auto");
        }
        if (parentRow)
        {
            if (toNumber(this.style.maxWidth,1) > toNumber(this.style.minWidth,-1))
                this.style.setProperty("flex","1 1 auto");
            else
                this.style.setProperty("flex","0 0 auto");
        }
    }
    if (parentLayers)
    {
        this.style.setProperty("position","absolute");
        this.style.setProperty("top","0");
        this.style.setProperty("left","0");
        this.style.setProperty("height","100%");
        this.style.setProperty("width","100%");
        //z-index?
    }
    if (body)
    {
          document.body.style.maxWidth = this.style.maxWidth;
          document.body.style.maxHeight = this.style.maxHeight;
          document.body.style.minWidth = this.style.minWidth;
          document.body.style.minHeight = this.style.minHeight;
    }

    transferUilStyle.call(this);
}

function uilAppendChild(parent,child)
{
    parent.appendChild(child);
    if (!parent.uilName)
    {
        while (parent && parent != document && !parent.uilName)
            parent = parent.parentElement;
    }
    else
    {
        let contentMarker = parent.getElementsByClassName(":content")[0];
        if (contentMarker)
        {
            child.parentElement?.removeChild(child);
            contentMarker.parentElement.insertBefore(child,contentMarker);
        }
    }
    parent.uilChildren.push(child);
    DOMTraverse.call(parent);
    uilTraverse.call(parent);
}

function uilRemoveChild(parent,child)
{
    child.parentNode.removeChild(child);
    if (!parent.uilName)
    {
        while (parent && parent != document && !parent.uilName)
            parent = parent.parentElement;
    }
    parent.uilChildren = parent.uilChildren.filter(element => element !== child);
    DOMTraverse.call(parent);
    uilTraverse.call(parent);
}

function debugHook()
{
    return null;
}
0