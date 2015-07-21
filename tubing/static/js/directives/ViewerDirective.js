angular.module('fishApp').directive('fishViewer', ['$log', '$window', 'Sim', function($log, $window, Sim) {
    return {
        restrict: 'E',
        templateUrl: '/static/template/fish_viewer.html',
        scope: {
            map: '='
        },
        link: function(scope, element, attrs) {
            $log.info("The directive is linking!");

            var container;
            var renderer, ocean;
            var text, plane;
            var simtime = 0;
            var txt;

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
                element.bind('resize', scope._on_window_resize);
                return
            }

            scope._setup_window_size = function() {
                scope.window_width = element.width();
                scope.window_height = 3*scope.window_width / 4;
                element.height(scope.window_height);
                return;
            }

            scope._on_window_resize = function() {
                $log.info("Resizing the window");

                scope.window_width = element.width();
                scope.window_height = element.height();

                windowHalfX = scope.window_width / 2;
                windowHalfY = scope.window_height / 2;

                scope.camera.aspect = scope.window_width / scope.window_height;
                scope.camera.updateProjectionMatrix();

                renderer.setSize( scope.window_width, scope.window_height );
                return;
            }

            scope._clear_scene = function() {
                scope.scene = new THREE.Scene();
                return;
            }

            scope._calculate_optimum_tile_size = function(mw, mh, ww, wh) {
                // 25px margin on each side
                var boundary_margin = 25;

                var tile_vertical_ratio = (wh-boundary_margin*2) / mh;
                var tile_horizontal_ratio = (ww-boundary_margin*2) / mw;

                return Math.min(tile_vertical_ratio, tile_horizontal_ratio);
            }

            scope._generate_map_scene = function() {
                var map = scope.map.map;
                scope.map_width = map[0].length;
                scope.map_height = map.length;

                // We need this to scale the tiles appropriately
                scope.obstacle_tile_size = scope._calculate_optimum_tile_size(scope.map_width,
                                                                              scope.map_height,
                                                                              scope.window_width,
                                                                              scope.window_height);

                scope.group_obstacle = new THREE.Group();
                scope.scene.add(scope.group_obstacle);

                scope.generate_obstacle_rect(scope.obstacle_tile_size, 0, 0);

                // Loop over each point and create the obstacles
                var x = 0, y = 0;
                for(y=0;y<scope.map_height;y++) {
                    for(x=0;x<scope.map_width;x++) {
                        var map_block_type = map[y][x];

                        if(map_block_type == '.') {
                            continue;
                        }

                        if(map_block_type=='#') {
                            scope.generate_obstacle_rect(scope.obstacle_tile_size, x, y);
                        }
                    }
                }

                return;
            }

            scope.generate_obstacle_rect = function(size, tile_x, tile_y) {
                var rectShape = new THREE.Shape();
                rectShape.moveTo( 0,0 );
                rectShape.lineTo( 0, size);
                rectShape.lineTo( size , size );
                rectShape.lineTo( size, 0 );
                rectShape.lineTo( 0, 0 );
                var margin = 25;
                var transformed_rect = scope._addShape( 0, rectShape, 0x9CD3DB, margin + tile_x*size, 0, margin + tile_y*size, 0, 0, 0, 1 );
                scope.group_obstacle.add(transformed_rect);
                return;
            }


            // scope.<something> is a way of combining this directive (the <fish-viewer> tag) with this function
            scope.init = function() {
                txt = datalog2.split(",");
                //alert(txt);
                // The initial window setup
                scope._setup_window_size();

                // Create a camera
                scope.camera = new THREE.OrthographicCamera( 0,     // left
                                                             scope.window_width,      // right
                                                             0,      // top
                                                             scope.window_height ,   // bottom
                                                             -500,                        // near
                                                             1000 );                      // far

                // Position the camera
                //scope.camera.position.set( scope.window_width/2, scope.window_height/2, 500);
                scope.camera.position.set( 0, 0, 1000);

                scope.scene = new THREE.Scene();
                scope.group = new THREE.Group();

                scope.group.position.y = 50;
                scope.scene.add( scope.group );

                renderer = new THREE.WebGLRenderer();
                renderer.setClearColor( 0xf0f0f0 );
                renderer.setPixelRatio( $window.devicePixelRatio );
                renderer.setSize( scope.window_width, scope.window_width );
                renderer.sortElements = false;
                renderer.context.getExtension('OES_texture_float');
                renderer.context.getExtension('OES_texture_float_linear');

                // Angular gives you "element" - which is the DOM element this code is being run 
                element.append( renderer.domElement );
                angular.element(renderer.domElement).addClass('col-md-12');

                // For run this once to fix aspect ratio
                scope._on_window_resize();

                // We just need to look for events on the element
                scope._setup_event_listener();
                //testSimple();

            }

                
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

            scope._addShape = function( id, shape, color, x, y, z, rx, ry, rz, s , enableBoundary, texture1) {
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

                // Added the next like to make sure things show up even though we've set
                // reverse coordinates in the vertical direction
                material.side = THREE.DoubleSide;

                var mesh = new THREE.Mesh( geometry, material );
                //mesh.position.set( x, y, z );
                var axis = new THREE.Vector3( 0, 1, 1 );
                mesh.rotation.set( rx, ry, rz );
                mesh.scale.set( s, s, s );
                
                var transform = new THREE.Group();
                transform.add(mesh);
                transform.position.set( x, z, y );
                idShapeMap[id] = transform;

                if (enableBoundary) {
                    // line

                    var geometry = shape.createPointsGeometry();
                    var material = new THREE.LineBasicMaterial( { linewidth: 10, color: 0x333333, transparent: true } );

                    var line = new THREE.Line( geometry, material );
                    line.position.set( x, y, z );
                    line.rotation.set( rx, ry, rz );
                    line.scale.set( s, s, s );
                    transform.add( line );
                }
                return transform;
            }
            
            function addSpriteShape( id, shape, color, x, y, z, rx, ry, rz, s , enableBoundary) {
                enableBoundary = typeof enableBoundary !== 'undefined' ? enableBoundary : false;
                // flat shape
                texture = THREE.ImageUtils.loadTexture("/static/img/babelfish.png");
                var material = new THREE.SpriteMaterial( { map: texture } );

                var width = 128;
                var height = 128;
            
                mesh = new THREE.Sprite( material );
                //mesh.scale.set( width, height, 1 );
                
                //mesh.position.set( x, y, z );
                var axis = new THREE.Vector3( 0, 1, 1 );
                mesh.rotation.set( rx, ry, rz );

                mesh.position.set( x, z, y );
                mesh.scale.set( s, s, s );

                idShapeMap[id] = mesh;
                scope.group.add( mesh );
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
                var transform = scope._addShape( id, fishShape, 0x222222, x, y, 0, rx, 0, ry, s );
                scope.group.add(transform);
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
                
                addSpriteShape( id, debugShape, 0xAAAAAA, x, y, 0, rx, 0, ry, s, "static/img/babelfish.png");    
                
                        
            }
            
            function addFrame() {
                // Rectangle frame Window
                var rectShape = new THREE.Shape();
                rectShape.moveTo( 0,0 );
                rectShape.lineTo( 0, window.innerWidth - 0 );
                rectShape.lineTo( window.innerHeight - 0 , window.innerWidth - 0 );
                rectShape.lineTo( window.innerHeight - 0, 0 );
                rectShape.lineTo( 0, 0 );
                var transform = scope._addShape( 0, rectShape, 0x9CD3DB, 0, 0, 0, 0, 0, 0, 1 );
                scope.group.add(transform)
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
                scope._addShape( id, obsShape, 0x000000, 0, 0, 0, 0, 0, 0, 1 );
            }
            
            function readTextFile(file)
            {
                var rawFile = new XMLHttpRequest();
                rawFile.open("GET", file, false);
                rawFile.onreadystatechange = function ()
                {
                    if(rawFile.readyState === 4)
                    {
                        if(rawFile.status === 200 || rawFile.status == 0)
                        {
                            var allText = rawFile.responseText;
                            alert(allText);
                        }
                    }
                }
                rawFile.send(null);
            }
            
            function testSimple() {
                //addFrame();
                var points = [200, 200, 250, 200, 250, 250, 300, 250, 300, 300, 200, 300];
                //addObstacles(1, points);
                //addfish1(2, 0, 0, 0, Math.PI, 1);   
                addfishBlock(2, 0, 0, 0, Math.PI/2, scope.obstacle_tile_size);
                addfishBlock(3, 0, 0, 0, Math.PI/2, scope.obstacle_tile_size); 
                addfishBlock(4, 0, 0, 0, Math.PI/2, scope.obstacle_tile_size);            

                var margin = 25;
                var geometry = new THREE.Geometry();
                for (i = 0; i < txt.length; i = i+8) { 
                     geometry.vertices.push(new THREE.Vector3(margin + txt[i]*scope.obstacle_tile_size, margin + txt[i+1]*scope.obstacle_tile_size, 0));
                }
                var material = new THREE.LineBasicMaterial({
                        color: 0x0000ff
                });

                var line = new THREE.Line(geometry, material);
                scope.scene.add(line);
                scope.scene.add(scope.group);
            }               
            
            function testfishOrientation() {
                
            }
        
            function testObstacles() {
            
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
                        
                    var shapeId = 2; // XXX - hardcoded for testing purpose
                    var shapeId2 = 3;
                    var shapeId3 = 4;
                    
                    // TBD - get the data from network
                
                    var shape = idShapeMap[shapeId];
                    var shape2 = idShapeMap[shapeId2];
                    var shape3 = idShapeMap[shapeId3];
                    
                    /*
                    // XXX - testing code 
                    shape.position.x = shape.position.x + 1;
                    shape.position.y = shape.position.y + 1;
                    shape.material.rotation = shape.material.rotation + 0.01;
                    
                    shape2.position.x = shape2.position.x + 1;
                    shape2.position.y = shape2.position.y + 2;
                    shape2.rotation.z = shape2.rotation.z + 0.01;
                    */
                    simtime++; 
                    //console.log("Simtime is : " + simtime);
                    var index = Math.floor((simtime/100))*8;
                    //console.log("index is : " + index);
                    shape.position.x = parseInt(txt[index+2])*scope.obstacle_tile_size;
                    shape.position.y = parseInt(txt[index+3])*scope.obstacle_tile_size;
                    //console.log("Fish is at " + txt[index+2] + "," + (txt[index+3]) + " at simtime " + simtime);
                     
                    shape2.position.x = parseInt(txt[index+4])*scope.obstacle_tile_size;
                    shape2.position.y = parseInt(txt[index+5])*scope.obstacle_tile_size;
                    
                    shape3.position.x = parseInt(txt[index+6])*scope.obstacle_tile_size;
                    shape3.position.y = parseInt(txt[index+7])*scope.obstacle_tile_size;
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

            // Everytime the map changes, we need to reset everything
            scope.$watch('map', function(newval, oldval) {
                if(!newval) {
                    return;
                }

                scope._clear_scene();
                scope._generate_map_scene();
                testSimple();
            }, true);

            scope.$watch(function() {
                return Sim.get_fish_count();
            }, function(newval, oldval) {
                if(newval == 0) {
                    return;
                }
                $log.log("New fish created: " + newval);
                var current_frame = Sim.get_current_frame();
                var pos = Sim.get_fish_position(current_frame, newval-1);
                debugger
                scope.addfish1(newval-1, 25 + pos.pos_x, 25 + pos.pos_y, 0, 0, 0.05);
            });

            scope.init();
            scope.animate();
        }
    };
}]);
