# This can be used to create a Docker container with a prebuilt 3D Slicer
# Run this to create a docker container called slicer-testbuild:
# docker build -t slicer-testbuild .

FROM slicer/slicer-base:latest
RUN \
	#ls	  
	# Checkout latest
		cd /usr/src && \
		git clone git://github.com/Slicer/Slicer && \
		cd Slicer && \
		#git pull && \
		# Make sure we have a version that actually builds
		git checkout v4.11 && \
		
	# Re-configure
		#cd /usr/src/Slicer-build && \
		#cmake -DBUILD_TESTING:BOOL=OFF . && \
	# Build
		cd /usr/src/Slicer-build && \/usr/src/Slicer-build/BuildSlicer.sh 
		
WORKDIR /usr/src/Slicer-build
CMD bash