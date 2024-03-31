using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DroneLineOfSight : MonoBehaviour
{
    private List<Collider> objectsInView = new();

    private void OnTriggerEnter(Collider other) {
        if(other.CompareTag("Wall") || other.CompareTag("Player") || other.CompareTag("Crate") || other.CompareTag("Drone")){
            objectsInView.Add(other);
        }
    }


    private void OnTriggerExit(Collider other) {
        if (other.CompareTag("Wall") || other.CompareTag("Player") || other.CompareTag("Crate") || other.CompareTag("Drone")){
            objectsInView.Remove(other);
        }
    }

    private void Update() {
        if (objectsInView.Count > 0) {
            int closest = 0;
            for (int i = 1; i < objectsInView.Count; i++) {
                if(Vector3.Distance(objectsInView[i].transform.position, transform.parent.GetChild(0).position) < Vector3.Distance(objectsInView[closest].transform.position, transform.parent.GetChild(0).position)) {
                    closest = i;
                }
            }
            if (objectsInView[closest].CompareTag("Player")) {
                objectsInView[closest].transform.parent.GetComponent<PlayerBehavior>().GameOver();
            }
        }
    }
}
