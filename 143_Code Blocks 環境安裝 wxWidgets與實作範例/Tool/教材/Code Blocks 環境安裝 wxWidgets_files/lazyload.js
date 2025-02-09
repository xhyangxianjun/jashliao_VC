/*
  lazyload.js: Image lazy loading

  Copyright (c) 2012 Vincent Voyer, http://fasterize.com

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

// Prevent double lazyload script on same page
// We NEED to use string as closure compiler would otherwise compile this statement badly
if (!window['lzld']) {
  (function(window, document){
    var
      // Vertical offset in px. Used for preloading images while scrolling
      offset = 100,
      //where to get real src
      lazyAttr = 'data-src',
      // Window height
      winH = viewport(),
      // Self-populated page images array, we do not getElementsByTagName
      imgs = [],
      pageHasLoaded = false,
      unsubscribed = false,

      // throttled functions, so that we do not call them too much
      saveViewportT = throttle(viewport, 20),
      showImagesT = throttle(showImages, 20);

    // Override image element .getAttribute globally so that we give the real src
    // does not works for ie < 8: http://perfectionkills.com/whats-wrong-with-extending-the-dom/
    // Internet Explorer 7 (and below) [...] does not expose global Node, Element, HTMLElement, HTMLParagraphElement
    window['HTMLImageElement'] && overrideGetattribute();

    // Called from every lazy <img> onload event
    window['lzld'] = onDataSrcImgLoad;

    // init
    domready(findImages);
    addEvent(window, 'load', onLoad);

    // Bind events
    subscribe();

    // called by img onload= or onerror= for IE6/7
    function onDataSrcImgLoad(img) {
      // if image is not already in the imgs array
      // it can already be in it if domready was fast and img onload slow
      if (inArray(img, imgs) === -1) {
        // this case happens when the page had loaded but we inserted more lazyload images with
        // javascript (ajax). We need to re-watch scroll/resize
        if (unsubscribed) {
          subscribe();
        }
        showIfVisible(img, imgs.push(img) - 1);
      }
    }

    // find and merge images on domready with possibly already present onload= onerror= imgs
    function findImages() {
      var
        domreadyImgs = document.getElementsByTagName('img'),
        currentImg;

      // merge them with already self onload registered imgs
      for (var imgIndex = 0, max = domreadyImgs.length; imgIndex < max; imgIndex += 1) {
        currentImg = domreadyImgs[imgIndex];
        if (currentImg.getAttribute(lazyAttr) && inArray(currentImg, imgs) === -1) {
          imgs.push(currentImg);
        }
      }

      showImages();
      setTimeout(showImagesT, 25);
    }

    function onLoad() {
      pageHasLoaded = true;
      // if page height changes (hiding elements at start)
      // we should recheck for new in viewport images that need to be shown
      // see onload test
      showImagesT();
      // we could be the first to be notified about onload, so let others event handlers
      // pass and then try again
      // because they could change things on images
      setTimeout(showImagesT, 25);
    }

    function throttle(fn, minDelay) {
      var lastCall = 0;
      return function() {
        var now = +new Date();
        if (now - lastCall < minDelay) {
          return;
        }
        lastCall = now;
        // we do not return anything as
        // https://github.com/documentcloud/underscore/issues/387
        fn.apply(this, arguments);
      }
    }

    // X-browser
    function addEvent( el, type, fn ) {
      if (el.attachEvent) {
        el.attachEvent && el.attachEvent( 'on' + type, fn );
      } else {
        el.addEventListener( type, fn, false );
      }
    }

    // X-browser
    function removeEvent( el, type, fn ) {
      if (el.detachEvent) {
        el.detachEvent && el.detachEvent( 'on' + type, fn );
      } else {
        el.removeEventListener( type, fn, false );
      }
    }

    // custom domready function
    // ripped from https://github.com/dperini/ContentLoaded/blob/master/src/contentloaded.js
    // http://javascript.nwbox.com/ContentLoaded/
    // http://javascript.nwbox.com/ContentLoaded/MIT-LICENSE
    // kept the inner logic, merged with our helpers/variables
    function domready(callback) {
      var
        done = false,
        top = true;

      function init(e) {
        if (e.type === 'readystatechange' && document.readyState !== 'complete') return;
        removeEvent((e.type === 'load' ? window : document), e.type, init);
        if (!done) {
          done = true;
          callback();
        }
      }

      function poll() {
        try { document.documentElement.doScroll('left'); } catch(e) { setTimeout(poll, 50); return; }
        init('poll');
      }

      if (document.readyState === 'complete') callback();
      else {
        if (document.createEventObject && document.documentElement.doScroll) {
          try { top = !window.frameElement; } catch(e) { }
          if (top) poll();
        }
        addEvent(document, 'DOMContentLoaded', init);
        addEvent(document, 'readystatechange', init);
        addEvent(window, 'load', init);
      }

    }

    // img = dom element
    // index = imgs array index
    function showIfVisible(img, index) {
      // We have to check that the current node is in the DOM
      // It could be a detached() dom node
      // http://bugs.jquery.com/ticket/4996
      if (contains(document.documentElement, img)
        && img.getBoundingClientRect().top < winH + offset) {
        // To avoid onload loop calls
        // removeAttribute on IE is not enough to prevent the event to fire
        img.onload = null;
        img.removeAttribute('onload');
        // on IE < 8 we get an onerror event instead of an onload event
        img.onerror = null;
        img.removeAttribute('onerror');

        //img.src = img.getAttribute(lazyAttr);
        //console.log($(img));
        $(img).hide().attr("src",img.getAttribute(lazyAttr)).fadeIn('slow');
        img.removeAttribute(lazyAttr);
        imgs[index] = null;

        return true; // img shown
      } else {
        return false; // img to be shown
      }
    }

    // cross browser viewport calculation
    function viewport() {
      if (document.documentElement.clientHeight >= 0) {
        return document.documentElement.clientHeight;
      } else if (document.body && document.body.clientHeight >= 0) {
        return document.body.clientHeight
      } else if (window.innerHeight >= 0) {
        return window.innerHeight;
      } else {
        return 0;
      }
    }

    function saveViewport() {
      winH = viewport();
    }

    // Loop through images array to find to-be-shown images
    function showImages() {
      var
        last = imgs.length,
        current,
        allImagesDone = true;

      for (current = 0; current < last; current++) {
        var img = imgs[current];
        // if showIfVisible is false, it means we have some waiting images to be
        // shown
        if(img !== null && !showIfVisible(img, current)) {
          allImagesDone = false;
        }
      }

      if (allImagesDone && pageHasLoaded) {
        unsubscribe();
      }
    }

    function unsubscribe() {
      unsubscribed = true;
      removeEvent(window, 'resize', saveViewportT);
      removeEvent(window, 'scroll', showImagesT);
      removeEvent(window, 'load', onLoad);
    }

    function subscribe() {
      unsubscribed = false;
      addEvent(window, 'resize', saveViewportT);
      addEvent(window, 'scroll', showImagesT);
    }

    function overrideGetattribute() {
      var original = HTMLImageElement.prototype.getAttribute;
      HTMLImageElement.prototype.getAttribute = function(name) {
        if(name === 'src') {
          var realSrc = original.call(this, lazyAttr);
          return realSrc || original.call(this, name);
        } else {
          // our own lazyloader will go through theses lines
          // because we use getAttribute(lazyAttr)
          return original.call(this, name);
        }
      }
    }

    // https://github.com/jquery/sizzle/blob/3136f48b90e3edc84cbaaa6f6f7734ef03775a07/sizzle.js#L708
    var contains = document.documentElement.compareDocumentPosition ?
      function( a, b ) {
        return !!(a.compareDocumentPosition( b ) & 16);
      } :
      document.documentElement.contains ?
      function( a, b ) {
        return a !== b && ( a.contains ? a.contains( b ) : false );
      } :
      function( a, b ) {
        while ( (b = b.parentNode) ) {
          if ( b === a ) {
            return true;
          }
        }
        return false;
      };

    // https://github.com/jquery/jquery/blob/f3515b735e4ee00bb686922b2e1565934da845f8/src/core.js#L610
    // We cannot use Array.prototype.indexOf because it's not always available
    function inArray(elem, array, i) {
      var len;

      if ( array ) {
        if ( Array.prototype.indexOf ) {
          return Array.prototype.indexOf.call( array, elem, i );
        }

        len = array.length;
        i = i ? i < 0 ? Math.max( 0, len + i ) : i : 0;

        for ( ; i < len; i++ ) {
          // Skip accessing in sparse arrays
          if ( i in array && array[ i ] === elem ) {
            return i;
          }
        }
      }

      return -1;
    }

  }(this, document))
}
