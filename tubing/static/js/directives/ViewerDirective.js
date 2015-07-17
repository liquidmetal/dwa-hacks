angular.module('fishApp').directive('fishViewer', ['$log', '$window', function($log, $window) {
    return {
        restrict: 'E',
        templateUrl: '/static/template/fish_viewer.html',
        scope: {
            something: '@'
        },
        link: function(scope, element, attrs) {
            $log.info("The directive is linking!");

            var container;
            var renderer, ocean;
            var text, plane;

            scope.targetRotation = 0;
            scope.targetRotationOnMouseDown = 0;

            scope.mouseX = 0;
            scope.mouseXOnMouseDown = 0;

            // In angular land, use $window instead of just window
            var windowHalfX = $window.innerWidth / 2;
            var windowHalfY = $window.innerHeight / 2;
            var listTime = (new Date()).getTime();

            var idShapeMap = {};

            scope._setup_event_listener = function() {
                element.bind('mousedown', onDocumentMouseDown);
                element.bind('touchstart', onDocumentTouchStart);
                element.bind('touchmove', onDocumentTouchMove);
                element.bind('resize', onWindowResize);
                return
            }

            // scope.<something> is a way of combining this directive (the <fish-viewer> tag) with this function
            scope.updateOcean = function() {
                var currentTime = new Date().getTime();
                ocean.deltaTime = (currentTime - lastTime) / 1000 || 0.0;
                lastTime = currentTime;
                ocean.render(ocean.deltaTime);
                ocean.overrideMaterial = ocean.materialOcean;
                if (ocean.changed) {
                    ocean.materialOcean.uniforms.u_size.value = ocean.size;
                    ocean.materialOcean.uniforms.u_sunDirection.value.set( ocean.sunDirectionX, ocean.sunDirectionY, ocean.sunDirectionZ );
                    ocean.materialOcean.uniforms.u_exposure.value = ocean.exposure;
                    ocean.changed = false;
                }
                ocean.materialOcean.uniforms.u_normalMap.value = ocean.normalMapFramebuffer ;
                ocean.materialOcean.uniforms.u_displacementMap.value = ocean.displacementMapFramebuffer ;
                ocean.materialOcean.uniforms.u_projectionMatrix.value = scope.camera.projectionMatrix ;
                ocean.materialOcean.uniforms.u_viewMatrix.value = scope.camera.matrixWorldInverse ;
                ocean.materialOcean.uniforms.u_cameraPosition.value = scope.camera.position;
                ocean.materialOcean.depthTest = true;
                //scene.__lights[1].position.x = scene.__lights[1].position.x + 0.01;
                //Display();
            }

            scope.init = function() {
                // Create a camera
                scope.camera = new THREE.OrthographicCamera( scope.window_width / -2,     // left
                                                       scope.window_width / 2,      // right
                                                       scope.window_height/ 2,      // top
                                                       scope.window_height / - 2,   // bottom
                                                       -500,                        // near
                                                       1000 );                      // far

                // Position the camera
                scope.camera.position.set( scope.window_width/4, scope.window_height/2, 500);

                scope.scene = new THREE.Scene();
                scope.group = new THREE.Group();

                scope.group.position.y = 50;
                scope.scene.add( scope.group );
                
                var rotObjectMatrix;
                function rotateAroundObjectAxis(object, axis, radians) {
                    rotObjectMatrix = new THREE.Matrix4();
                    rotObjectMatrix.makeRotationAxis(axis.normalize(), radians);
                
                    // old code for Three.JS pre r54:
                    // object.matrix.multiplySelf(rotObjectMatrix);      // post-multiply
                    // new code for Three.JS r55+:
                    object.matrix.multiply(rotObjectMatrix);
                
                    // old code for Three.js pre r49:
                    // object.rotation.getRotationFromMatrix(object.matrix, object.scale);
                    // old code for Three.js r50-r58:
                    // object.rotation.setEulerFromRotationMatrix(object.matrix);
                    // new code for Three.js r59+:
                    object.rotation.setFromRotationMatrix(object.matrix);
                }
                
                var rotWorldMatrix;
                // Rotate an object around an arbitrary axis in world space       
                function rotateAroundWorldAxis(object, axis, radians) {
                    rotWorldMatrix = new THREE.Matrix4();
                    rotWorldMatrix.makeRotationAxis(axis.normalize(), radians);
                
                    // old code for Three.JS pre r54:
                    //  rotWorldMatrix.multiply(object.matrix);
                    // new code for Three.JS r55+:
                    rotWorldMatrix.multiply(object.matrix);                // pre-multiply
                
                    object.matrix = rotWorldMatrix;
                
                    // old code for Three.js pre r49:
                    // object.rotation.getRotationFromMatrix(object.matrix, object.scale);
                    // old code for Three.js pre r59:
                    // object.rotation.setEulerFromRotationMatrix(object.matrix);
                    // code for r59+:
                    object.rotation.setFromRotationMatrix(object.matrix);
                }

                function addShape( id, shape, color, x, y, z, rx, ry, rz, s , enableBoundary, texture1) {
                    enableBoundary = typeof enableBoundary !== 'undefined' ? enableBoundary : false;
                    // flat shape

                    var geometry = new THREE.ShapeGeometry( shape );
                    var material;
                    
                    if (texture1 !== 'undefined') {
                        console.log("texture valid");
                        THREE.ImageUtils.crossOrigin = 'anonymous';
                        sprite = THREE.ImageUtils.loadTexture("/static/img/babelfish.png");
                        material = new THREE.MeshBasicMaterial( { color: color, overdraw: 0.5, map: sprite} );
                    } else {
                        console.log("texture not valid");
                        material = new THREE.MeshBasicMaterial( { color: color, overdraw: 0.5} );
                    }

                    var mesh = new THREE.Mesh( geometry, material );
                    //mesh.position.set( x, y, z );
                    var axis = new THREE.Vector3( 0, 1, 1 );
                    mesh.rotation.set( rx, ry, rz );
                    mesh.scale.set( s, s, s );
                    
                    var transform = new THREE.Group();
                    transform.add(mesh);
                    transform.position.set( x, z, y );
                    idShapeMap[id] = transform;
                    scope.group.add( transform );

                    if (enableBoundary) {
                        // line

                        var geometry = shape.createPointsGeometry();
                        var material = new THREE.LineBasicMaterial( { linewidth: 10, color: 0x333333, transparent: true } );

                        var line = new THREE.Line( geometry, material );
                        line.position.set( x, y, z );
                        line.rotation.set( rx, ry, rz );
                        line.scale.set( s, s, s );
                        scope.group.add( line );
                    }
                }
                
                function addSpriteShape( id, shape, color, x, y, z, rx, ry, rz, s , enableBoundary) {
                    enableBoundary = typeof enableBoundary !== 'undefined' ? enableBoundary : false;
                    // flat shape
                    texture = THREE.ImageUtils.loadTexture("/static/img/babelfish.png");
                    var material = new THREE.SpriteMaterial( { map: texture } );

                    var width = 128;
                    var height = 128;
                
                    mesh = new THREE.Sprite( material );
                    mesh.scale.set( width, height, 1 );
                    
                    //mesh.position.set( x, y, z );
                    var axis = new THREE.Vector3( 0, 1, 1 );
                    mesh.rotation.set( rx, ry, rz );

                    mesh.position.set( x, z, y );
                    idShapeMap[id] = mesh;
                    scope.group.add( mesh );

                    if (enableBoundary) {
                        // line

                        var geometry = shape.createPointsGeometry();
                        var material = new THREE.LineBasicMaterial( { linewidth: 10, color: 0x333333, transparent: true } );

                        var line = new THREE.Line( geometry, material );
                        line.position.set( x, y, z );
                        line.rotation.set( rx, ry, rz );
                        line.scale.set( s, s, s );
                        scope.group.add( line );
                    }
                }
                
                function addfish1(id, x, y, rx, ry, s) {
                    // Fish
                    //x = y = 0;
                    rx = typeof rx !== 'undefined' ? rx : 0;
                    ry = typeof ry !== 'undefined' ? ry : 0;
                    s = typeof s !== 'undefined' ? s : 1;
                    
                    var debugShape = new THREE.Shape();
                    
                    debugShape.moveTo(x, y - 80);
                    debugShape.lineTo(x + 115, y - 80);
                    debugShape.lineTo(x + 115, y + 80);
                    debugShape.lineTo(x, y + 80);
                    debugShape.lineTo(x, y - 80);
                    //addShape( debugShape, 0xAAAAAA, 0, 0, 0, rx, 0, ry, s );
                    
                    var fishShape = new THREE.Shape();

                    fishShape.moveTo(-57.5,0);
                    fishShape.quadraticCurveTo(-7.5, -80, 32.5, -10);
                    fishShape.quadraticCurveTo(42.5, -10, 57.5, -40);
                    fishShape.quadraticCurveTo(57.5, 0, 57.5, 40);
                    fishShape.quadraticCurveTo(42.5, 10, 32.5, 10);
                    fishShape.quadraticCurveTo(-7.5, 80, -57.5, 0);
                    addShape( id, fishShape, 0x222222, x, y, 0, rx, 0, ry, s );
                }
                
                function addfishBlock(id, x, y, rx, ry, s) {
                    // Fish
                    //x = y = 0;
                    rx = typeof rx !== 'undefined' ? rx : 0;
                    ry = typeof ry !== 'undefined' ? ry : 0;
                    s = typeof s !== 'undefined' ? s : 1;
                    
                    var debugShape = new THREE.Shape();
                    
                    debugShape.moveTo(-6, -8);
                    debugShape.lineTo(6, -8);
                    debugShape.lineTo(6, 8);
                    debugShape.lineTo(-6, 8);
                    debugShape.lineTo(-6, -8);
                    
                    addSpriteShape( id, debugShape, 0xAAAAAA, x, y, 0, rx, 0, ry, s, "http://localhost/iis-85.png");    
                    
                            
                }
                
                function addFrame() {
                    // Rectangle frame Window

                    var rectShape = new THREE.Shape();
                    rectShape.moveTo( 0,0 );
                    rectShape.lineTo( 0, window.innerWidth - 0 );
                    rectShape.lineTo( window.innerHeight - 0 , window.innerWidth - 0 );
                    rectShape.lineTo( window.innerHeight - 0, 0 );
                    rectShape.lineTo( 0, 0 );
                    addShape( 0, rectShape, 0x9CD3DB, 0, 0, 0, 0, 0, 0, 1 );
                }
                
                function addObstacles(id, pts) {
                    if (pts == 'undefined' || pts.length == 0) {
                        return;
                    }
                    
                    var obsShape = new THREE.Shape();
                    obsShape.moveTo(pts[0], pts[1]);
                    for (i = 2; i < pts.length; i = i + 2) { 
                        obsShape.lineTo(pts[i] , pts[i + 1]);
                    }
                    obsShape.lineTo(pts[0], pts[1]);
                    addShape( id, obsShape, 0x000000, 0, 0, 0, 0, 0, 0, 1 );
                }
                

                
                function createOcean() {
                    var gsize = 512; 
                    var res = 1024; 
                    var gres = res / 2;
                    var origx = -gsize / 2;
                    var origz = -gsize / 2;
                    ocean = new THREE.Ocean(renderer, scope.camera, scope.scene,
                    {
                        USE_HALF_FLOAT : true,
                        INITIAL_SIZE : 256.0,
                        INITIAL_WIND : [10.0, 10.0],
                        INITIAL_CHOPPINESS : 1.5,
                        GEOMETRY_ORIGIN : [0, 0],
                        SUN_DIRECTION : [-1.0, 1.0, 1.0],
                        OCEAN_COLOR: new THREE.Vector3(0.004, 0.016, 0.047),
                        SKY_COLOR: new THREE.Vector3(3.2, 9.6, 12.8),
                        EXPOSURE : 0.35,
                        GEOMETRY_RESOLUTION: gres,
                        GEOMETRY_SIZE : gsize,
                        RESOLUTION : res
                    });
                    ocean.materialOcean.uniforms.u_projectionMatrix = { type: "m4", value: scope.camera.projectionMatrix };
                    ocean.materialOcean.uniforms.u_viewMatrix = { type: "m4", value: scope.camera.matrixWorldInverse };
                    ocean.materialOcean.uniforms.u_cameraPosition = { type: "v3", value: scope.camera.position };
                    
                    var transform = new THREE.Group();
                    transform.add(ocean.oceanMesh);
                    transform.position.set( 100, 100, 0 );
                    
                    scope.scene.add(transform);
                }
                
                function testSimple() {
                    addFrame();
                    var points = [200, 200, 250, 200, 250, 250, 300, 250, 300, 300, 200, 300];
                    addObstacles(1, points);
                    addfish1(2, 500, 500, 0, Math.PI, 1);   
                    addfishBlock(3, 500, 500, 0, Math.PI/2, 5);             
                }               
                
                function testfishOrientation() {
                    
                }
            
                function testObstacles() {
                
                }

                testSimple();               

                renderer = new THREE.WebGLRenderer();
                renderer.setClearColor( 0xf0f0f0 );
                renderer.setPixelRatio( $window.devicePixelRatio );
                renderer.setSize( scope.window_width, scope.window_width );
                renderer.sortElements = false;
                renderer.context.getExtension('OES_texture_float');
                renderer.context.getExtension('OES_texture_float_linear');

                // Angular gives you "element" - which is the DOM element this code is being run 
                element.append( renderer.domElement );

                onWindowResize();

                // We just need to look for events on the element
                scope._setup_event_listener();
                createOcean();
                //testSimple();

            }

            function onWindowResize() {
                $log.info("Resizing the window");
                element.height(240);
                element.width(320);

                scope.window_width = element.width();
                scope.window_height = element.height();

                windowHalfX = scope.window_width / 2;
                windowHalfY = scope.window_height / 2;

                scope.camera.aspect = scope.window_width / scope.window_height;
                scope.camera.updateProjectionMatrix();

                renderer.setSize( scope.window_width, scope.window_width );
                return;
            }

            //

            function onDocumentMouseDown( event ) {

                event.preventDefault();

                document.addEventListener( 'mousemove', onDocumentMouseMove, false );
                document.addEventListener( 'mouseup', onDocumentMouseUp, false );
                document.addEventListener( 'mouseout', onDocumentMouseOut, false );

                scope.mouseXOnMouseDown = event.clientX - windowHalfX;
                scope.targetRotationOnMouseDown = scope.targetRotation;

            }

            function onDocumentMouseMove( event ) {

                scope.mouseX = event.clientX - windowHalfX;

                scope.targetRotation = scope.targetRotationOnMouseDown + ( scope.mouseX - scope.mouseXOnMouseDown ) * 0.02;

            }

            function onDocumentMouseUp( event ) {

                document.removeEventListener( 'mousemove', onDocumentMouseMove, false );
                document.removeEventListener( 'mouseup', onDocumentMouseUp, false );
                document.removeEventListener( 'mouseout', onDocumentMouseOut, false );

            }

            function onDocumentMouseOut( event ) {

                document.removeEventListener( 'mousemove', onDocumentMouseMove, false );
                document.removeEventListener( 'mouseup', onDocumentMouseUp, false );
                document.removeEventListener( 'mouseout', onDocumentMouseOut, false );

            }

            function onDocumentTouchStart( event ) {

                if ( event.touches.length == 1 ) {

                    event.preventDefault();

                    scope.mouseXOnMouseDown = event.touches[ 0 ].pageX - windowHalfX;
                    scope.targetRotationOnMouseDown = scope.targetRotation;

                }

            }

            function onDocumentTouchMove( event ) {

                if ( event.touches.length == 1 ) {

                    event.preventDefault();

                    scope.mouseX = event.touches[ 0 ].pageX - windowHalfX;
                    scope.targetRotation = scope.targetRotationOnMouseDown + ( scope.mouseX - scope.mouseXOnMouseDown ) * 0.05;

                }

            }
            
            scope.update = function() {
                // TBD - do this for every data packet received
                {
                    var shapeId = 3; // XXX - hardcoded for testing purpose
                    var shapeId2 = 2;
                    
                
                    // TBD - get the data from network
                
                    var shape = idShapeMap[shapeId];
                    var shape2 = idShapeMap[shapeId2];
                    
                    // XXX - testing code 
                    shape.position.x = shape.position.x + 1;
                    shape.position.y = shape.position.y + 1;
                    shape.material.rotation = shape.material.rotation + 0.01;
                    
                    shape2.position.x = shape2.position.x + 1;
                    shape2.position.y = shape2.position.y + 2;
                    shape2.rotation.z = shape2.rotation.z + 0.01;
                }
            }

            scope.animate = function() {
                requestAnimationFrame( scope.animate );
                scope.update();
                scope.render();
            }

            scope.render = function() {
                renderer.render( scope.scene, scope.camera );
            }

            scope.init();
            scope.animate();
        }
    };
}]);
