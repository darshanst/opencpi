#include <iostream>
#include "OcpiApi.h"
#include "OcpiContainerApi.h"
#include "OcpiPValueApi.h"


namespace OA = OCPI::API;
static const double pi = 3.14159265358979323846;


int main ( int argc, char* argv [ ] )
{
  std::string app_xml("<application>"
		      " <policy mapping='MaxProcessors' processors='0'/>"

		      "  <instance worker='file_read_msg' >"
		      "    <property name='fileName' value='dataIn.dat'/> "		      
		      "    <property name='genTestFile' value='true'/> "		      
		      "    <property name='stepThruMsg' value='true'/> "
		      "    <property name='stepNow' value='true'/> "
		      "  </instance> "



		      "  <instance worker='sym_fir_real' name='tx_fir_r' >"
		      "    <property name='bypass' value='false'/> "
		      "    <property name='gain' value='1'/> "
		      "  </instance> "

		      "  <instance worker='fsk_mod_complex' name='fsk_mod' >"
		      "  </instance> "

		      "  <instance worker='sym_fir_complex' name='tx_fir_c'>"
		      "    <property name='bypass' value='false'/> "
		      "    <property name='gain' value='1'/> "
		      "  </instance> "

		      "  <instance worker='cic_hpfilter_complex' name='tx_cic' >"
		      "    <property name='M' value='2'/> "
		      "  </instance> "
			 
		      "  <instance worker='loopback' >"
		      "  </instance> "

		      "  <instance worker='noise_gen_complex' >"
		      "    <property name='mask' value='0'/> "		      
		      "  </instance> "

		      "  <instance worker='dds_complex' name='ddc_dds' >"
		      "    <property name='phaseIncrement' value='12345678'/> "
		      "    <property name='syncPhase' value='0'/> "
		      "  </instance> "

		      "  <instance worker='mixer_complex' name='ddc_mixer' >"
		      "  </instance> "


		      "  <instance worker='cic_lpfilter_complex' name='rx_cic' >"
		      "    <property name='M' value='2'/> "
		      "  </instance> "

		      "  <instance worker='sym_fir_complex' name='rx_fir_c' >"
		      "    <property name='bypass' value='false'/> "
		      "    <property name='gain' value='1'/> "
		      "  </instance> "

		      "  <instance worker='fm_demod_complex' name='fm_demod' >"
		      "  </instance> "


		      "  <instance worker='sym_fir_real' name='rx_fir_r' >"
		      "    <property name='bypass' value='false'/> "
		      "    <property name='gain' value='1'/> "
		      "  </instance> "
"<!--"
"-->"

		      "  <instance worker='file_write_msg' >"
		      "    <property name='fileName' value='dataOut.dat'/> "		      
		      "  </instance> "

		      "  <connection>"
		      "    <port instance='file_read_msg' name='out'/>"
		      "    <port instance='tx_fir_r' name='in'/>"
		      "  </connection>"

		      "  <connection>"
		      "    <port instance='tx_fir_r' name='out'/>"
		      "    <port instance='fsk_mod' name='in'/>"
		      "  </connection>"

		      "  <connection>"
		      "    <port instance='tx_fir_c' name='in'/>"
		      "    <port instance='fsk_mod' name='out'/>"
		      "  </connection>"

		      "  <connection>"
		      "    <port instance='tx_fir_c' name='out'/>"
		      "    <port instance='tx_cic' name='in'/>"
		      "  </connection>"


		      "  <connection>"
		      "    <port instance='noise_gen_complex' name='in'/>"
		      "    <port instance='tx_cic' name='out'/>"
		      "  </connection>"

		      "  <connection>"
		      "    <port instance='noise_gen_complex' name='out'/>"
		      "    <port instance='loopback' name='in'/>"
		      "  </connection>"
			 

		      "  <connection>"
		      "    <port instance='loopback' name='out'/>"
		      "    <port instance='ddc_mixer' name='in_if'/>"
		      "  </connection>"


		      "  <connection>"
		      "    <port instance='rx_cic' name='in'/>"
		      "    <port instance='ddc_mixer' name='out'/>"
		      "  </connection>"


		      "<!--   Connections internal to the DDC -->"
		      "  <connection>"
		      "    <port instance='ddc_mixer' name='out_sync_only'/>"
		      "    <port instance='ddc_dds' name='in'/>"
		      "  </connection>"


		      "  <connection>"
		      "    <port instance='ddc_mixer' name='in_dds'/>"
		      "    <port instance='ddc_dds' name='out'/>"
		      "  </connection>"

		      "  <connection>"
		      "    <port instance='rx_cic' name='out'/>"
		      "    <port instance='rx_fir_c' name='in'/>"
		      "  </connection>"

		      "  <connection>"
		      "    <port instance='fm_demod' name='in'/>"
		      "    <port instance='rx_fir_c' name='out'/>"
		      "  </connection>"

		      "  <connection>"
		      "    <port instance='fm_demod' name='out'/>"
		      "    <port instance='rx_fir_r' name='in'/>"
		      "  </connection>"

		      "  <connection>"
		      "    <port instance='rx_fir_r' name='out'/>"
		      "    <port instance='file_write_msg' name='in'/>"
		      "  </connection>"			 

		      "</application>");
  

  OA::Application * app = NULL;
  try
    {      
      int nContainers = 1;
      // Create several containers to distribute the workers on
      for ( int n=0; n<nContainers; n++ ) {
	char buf[1024];
	sprintf(buf, "Rcc Container %d\n", n );
	(void)OA::ContainerManager::find("rcc",buf);
      }

      OCPI::API::PValue minp_policy[] = {
	OCPI::API::PVULong("MinProcessors",0),
	OCPI::API::PVEnd
      };


      
      app = new OA::Application( app_xml, minp_policy);	
      fprintf(stderr, "Application XML parsed and deployments (containers and implementations) chosen\n");


      app->initialize();
      fprintf(stderr, "Application established: containers, workers, connections all created\n");
      printf(">>> DONE Initializing!\n");


      /*
      std::string taps ("{-2,11,22,27,25,15,0,-14,-25,-29,-25,-13,2,19,31,35,29,15,-3,-22,-35,"
		      "-40,-34,-18,2,22,37,41,34,16,-6,-28,-43,-46,-35,-14,12,38,54,57,43,17,-14,-44,-63,-66,-51,-22,13,46,"
			"66,68,50,16,-23,-60,-80,-78,-53,-9,40,84,109,105,73,19,-43,-98,-130,-127,-91,-28,44,106,139,132,84,5,"
		      "-82,-154,-186,-166,-92,20,144,245,292,267,168,14,-157,-301,-377,-357,-239,-47,168,346,426,370,174,-123,"
			"-447,-699,-781,-623,-206,417,1128,1752,2094,1983,1317,106,-1510,-3263,-4777,-5633,-5435,-3884,-851,3582,"
			"9113,15238,21318,26666,30645,32767}");
      app->setProperty( "tx_fir_r", "taps" , taps.c_str() );
      */

      app->start();

      while ( 1 ) {
	std::string value;
	app->getProperty( "file_read_msg", "stepThruMsg", value);
        if ( value == "true" ) {
	  app->getProperty( "file_read_msg", "stepNow", value);
	  if ( value == "false" ) {
	    // wait for user
	    char c;
	    std::cout << "Hit any key to continue" << std::endl;
	    std::cin >> c;
	    app->setProperty("file_read_msg","stepNow","true");
	  }
	}

	sleep( 1 );
      }

}
catch ( const std::string& s )
  {
    std::cerr << "\n\nException(s): " << s << "\n" << std::endl;
  }
 catch ( std::exception& g )
   {
     std::cerr << "\nException(g): "
	       << typeid( g ).name( )
	       << " : "
	       << g.what ( )
	       << "\n"
	       << std::endl;
   }
 catch ( ... )
   {
     std::cerr << "\n\nException(u): unknown\n" << std::endl;
   }

return 0;
}

