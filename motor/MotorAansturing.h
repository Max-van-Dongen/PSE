
class MotorAansturing {
public:
  MotorAansturing();
  void begin(int l, int r);
  void vooruit();
  void achteruit();
  void links();
  void rechts();
  void vooruit_links();
  void vooruit_rechts();
  void achteruit_links();
  void achteruit_rechts();
  void langzaam_stop();
  void noodstop();
  void verander_snelheid(int l, int r);
  void versnellen_tot(int s);
  void vertragen_tot(int s);
  int geef_snelheid_links() const;
  int geef_snelheid_rechts() const;

private:
  int snelheid_links;
  int snelheid_rechts;
  void verander_richting(int l, int r);
};